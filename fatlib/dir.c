/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"
#include "uni_up.inc"

/*
 * short names are treated as if they have case information:
 *  - files are lowercase
 *  - directories start with uppercase, rest is lowercase
 */

/* READING DIR */

/* ret: <0 err, 0 eof, 1 ok */
int dir_slot(struct file *dir, int pos, u8 slot[32])
{
	struct io io;
	int v;
	io.pos = pos<<5;
	io.len = 32;
	io.data = slot;
	io.op = IOOP_READ;
	v = file_io(dir, &io);
	if(!v) v = io.len==0;
	return v;
}

/* ret: <0 err, 0 eof, 1 ok (no empt. slots), >1 ok (empt slots) */
static int get_next_slot(struct rd *rd)
{
	int nempt = 0, start = ++rd->pos;
	int v;

	for(;;) {
		v = dir_slot(rd->dir, rd->pos, rd->slot);
		if(v<1) break; /* eof or err */
		v = 0;
		if(rd->slot[0] == 0) break; /* eof */
		nempt++;
		if(rd->slot[0] != 0xE5)
			return nempt; /* ok */
		if(rd->empt_pos<0 && nempt >= rd->empt_need)
			rd->empt_pos = start;
		rd->pos++;
	}
	if(v>=0 && rd->empt_pos<0)
		rd->empt_pos = start;
	return v;
}

inline u8 calc_csum(u8 *p)
{
	int n = 10;
	u8 s = *p++;
	do {
		s = s<<7 | s>>1;
		s += *p++;
	} while(--n);
	return s;
}

/* lfn slot -> long name */
static int cp_uname(struct rd *rd, int slot)
{
	u16 *d = rd->name + 13*slot;
	int i=1, n=13;
	do {
		u16 c = get16(&rd->slot[i]); i+=2;
		if(!c) break;
		*d++ = c;
		if(i==11) i=14; else if(i==26) i=28;
	} while(--n);
	return 13-n;
}

#define ISLFN(P) ((P)[11]==0x0F)
#define LFNISSTART(P) (((P)[0]&0xC0)==0x40)

/* ret: <0 err, 0 eof, 1 read */
int read_entry(struct rd *rd)
{
	int v;

again:
	v = get_next_slot(rd);
	if(v<=0) return v;

read:
	rd->nlfn = 0;
	rd->nlen = 0;
	if(ISLFN(rd->slot)) {
		u8 csum;
		int ln;

		if(!LFNISSTART(rd->slot))
			goto again;

		csum = rd->slot[13];
		ln = rd->slot[0]&0x3F;
		if(!ln || ln>20)
			goto again;

		rd->nlfn = ln--;
		rd->nlen = 13*ln + cp_uname(rd, ln);
		if(!rd->nlen) goto again;

		while(ln) {
			v = get_next_slot(rd);
			if(v<=0) return v;
			if(v!=1) goto read;
			if(!ISLFN(rd->slot) || rd->slot[0]!=ln || rd->slot[13]!=csum)
				goto read;
			if(cp_uname(rd, --ln) != 13)
				goto again;
		}
		v = get_next_slot(rd);
		if(v<=0) return v;
		if(ISLFN(rd->slot)) goto read;
		if(v>1 || calc_csum(rd->slot) != csum)
			rd->nlfn = rd->nlen = 0;
	}
	return 1;
}

/* LOOKUP */

struct lookup;
static int file_creat(struct file **fptr, struct file *dir, int pos, struct lookup *lup, u8 attr);

struct lookup {
	int nlen; /* long name len */
	int dot; /* position of last dot (or -1)*/
	u8 flags;
#define LUP_NLOOKSH 1 /* (internal to lookup_init) */
#define LUP_INVENTSH 2 /* invent short name */
#define LUP_NOSHORT 3
	u8 sname[11];
	u16 lname[260];
};

/* short name from long name (lookup) */
static int to_short(DRIVE *dv, u8 *d, const u16 *s, int n)
{
	static const u32 x[] = {0xFC00DC04, 0x38000000, 1};
	do {
		unsigned c = *s++;
		if(c>='a' && c<='z') c -= 32;
		else if(c>='"' && c<='|') {
			if(x[(c>>5) - 1] & 1<<(c&0x1F))
				return 0;
		} else if(c >= 0x80) {
			c = uni_up(c);
			c = fat_u_to_cp(dv, c);
			if(!c) return 0;
		}
		*d++ = c;
	} while(--n);
	return 1;
}

static int unames_eq(u16 *a, u16 *b, int len)
{
	do {
		if(*a == *b)
			continue;
		if(uni_up(*a) != uni_up(*b))
			return 0;
	} while(a++, b++, --len);
	return 1;
}

static int lookup_init(DRIVE *dv, struct lookup *lup, const u8 *name, const u8 *nend)
{
	u16 *lname = lup->lname;
	const u8 *s = name;
	int n = 0;

	lup->flags = 0;
	lup->dot = -1;
	do {
		unsigned u = *s++;
		if(u >= 0x80) { /* utf-8 */
			u8 uu = u&0xF0;
			u8 c;

			lup->flags |= LUP_INVENTSH; /* no short name for create */
			u = u<<6 & 0xFC0;
			if(uu<0xC0 || uu>=0xF0 || s == nend)
				return -EINVAL;
			if(uu==0xE0) {
				c = *s++;
				if((c&0xC0)!=0x80 || s == nend)
					return -EINVAL;
				u |= (unsigned)c & 0x3F;
				u <<= 6;
			}
			c = *s++;
			if((c&0xC0)!=0x80)
				return -EINVAL;
			u |= c&0x3F;
		}
		lname[n] = u;
		if(u == '.') {
			if(lup->dot>=0) lup->flags = LUP_NOSHORT;
			lup->dot = n;
		}
		if(++n == elemof(lup->lname))
			return -ENAMETOOLONG;
	} while(s<nend);
	lup->nlen = n;

	if(lup->flags & LUP_NLOOKSH)
		goto noshort;

	memset(lup->sname+8, ' ', 3);
	if(lup->dot>=0) {
		n -= lup->dot+1;
		if(n > 3)
			goto noshort;
		if(!to_short(dv, lup->sname+8, lname+lup->dot+1, n))
			goto noshort;
		n = lup->dot;
	}
	if(!n || n > 8)
		goto noshort;

	memset(lup->sname, ' ', 8);
	if(!to_short(dv, lup->sname, lname, n)) {
noshort: /* no short name for lookup and create */
		lup->sname[0] = 0;
		lup->flags |= LUP_NOSHORT;
	}
	return 0;
}

/* move *fptr in directory tree, fput old node, fget new */
int lookup_one(struct file **fptr, const char *name, int nlen, int flags)
{
	struct file *dir = *fptr;
	struct rd rd;
	struct lookup lup;
	int v;

	DBG("lookup1 '%.*s' %04X\n", nlen, name, flags);

	if(*name=='.' && nlen<=2 && name[nlen-1]=='.') {
		struct file *parent = dir->dir;

		if(!(flags&LOOK_DIR))
			return -EISDIR;
		if(nlen > 1) {
			if(!parent) return -ENOENT;
			fget(parent); fput(dir);
			*fptr = parent;
		}
		return 0;
	}	

	v = lookup_init(dir->drive, &lup, name, name+nlen);
	if(v) return v;

	DBG("lookup_init: flags:%02X [%.11s]\n", lup.flags, lup.sname);
	rd.empt_pos = -1;
	rd.empt_need = (lup.nlen + 25)/13;

	rd.dir = dir;
	rd.pos = -1;

	for(;;) {
		v = read_entry(&rd);
		if(v<0) return v;
		if(v==0) goto not_found;
		if(rd.slot[11]&0x48)
			continue; /* skip label */

		if(*lup.sname && memcmp(rd.slot, lup.sname, 11)==0)
			break;
		if(lup.nlen != rd.nlen)
			continue;
		if(unames_eq(rd.name, lup.lname, lup.nlen))
			break;
	}

	if(flags&LOOK_EXCL)
		return -EEXIST;
	if(rd.slot[11]&0x10) {
		if(!(flags&LOOK_DIR))
			return -EISDIR;
	} else {
		if(!(flags&LOOK_FILE))
			return -ENOTDIR;
	}

	{
		list_t *l;
		for(l = dir->childs.next; l != &dir->childs; l = l->next) {
			struct file *file = list_entry(l, struct file, dirlst);
			if(file->entry == rd.pos) {
				fput(dir);
				*fptr = fget(file);
				return 0;
			}
		}
	}

	{
		struct file *file;
		long size = get32(rd.slot+28);
		clust_t clust = get16(rd.slot+26) | get16(rd.slot+20)<<16;
		file = file_alloc(dir->drive);
		if(!file) return -ENOMEM;
	
		file->dir = dir;
		file->entry = rd.pos;
		file->nlfn = rd.nlfn;
		file->size = size;
		file->clust = clust;
		memcpy(file->dirslot, rd.slot, sizeof file->dirslot);
	
		list_add(&dir->childs, &file->dirlst);

		*fptr = file;
		return 0;
	}

not_found:
	if(!(flags&LOOK_CREAT))
		return -ENOENT;

	/* empt_pos could be <0 on read error, but then we can't get here */
	assert(rd.empt_pos>=0);
	assert(!!(flags&LOOK_FILE) ^ !!(flags&LOOK_DIR));

	return file_creat(fptr, dir, rd.empt_pos, &lup,
		(flags&LOOK_DIR ? 0x10 : 0) | flags>>8);
}

/* WRITE */

static void insert_time(u8 *d, time_t t)
{
	struct tm tm;
	u32 v;

	localtime_r(&t, &tm);
	v = 0;
	if(tm.tm_year>=80)
		v = tm.tm_year-80<<25 | tm.tm_mon+1<<21 | tm.tm_mday<<16 |
		    tm.tm_hour<<11 | tm.tm_min<<5 | tm.tm_sec>>1;
	put32(d, v);
}

int file_write_meta(struct file *file)
{
	struct io io;
	DBG("file_write_meta: file {clust:%X entry:%u nlfn:%u '%.11s'}\n", file->clust, file->entry, file->nlfn, file->dirslot);

	list_del_init(&file->dirty); /* XXX even if error on write */
	if(file->dir) {
		unsigned size = file->size;
		clust_t clust = file->clust;
		put32(file->dirslot+0x1C, size);
		put16(file->dirslot+0x1A, clust);
		put16(file->dirslot+0x14, clust>>16);
		insert_time(file->dirslot+0x16, file->mtime);

		io.data = file->dirslot;
		io.pos = file->entry<<5;
		io.len = 32;
		io.op = IOOP_WRITE;
		file_io(file->dir, &io);
	}
	return 0;
}

/* CREAT */

void init_slot(u8 slot[32], const u8 *sname, u8 attr, int clust)
{
	memcpy(slot, sname, 11);
	slot[11] = attr;
	memset(slot+12, 0, 2);
	insert_time(slot+14, fat_time);
	memset(slot+18, 0, 2);
	put16(slot+20, clust>>16);
	memcpy(slot+22, slot+14, 4);
	put16(slot+26, clust);
	put32(slot+28, 0);
}

static int mkshort(u8 *buf, u16 *s, int n, int m)
{
#define SMAP1_START 32
#define SMAP1_END 127
	static const char smap1[] =
 /* 0020 */ "\0!_#$%&'()___-\0_0123456789______"
 /* 0040 */ "@ABCDEFGHIJKLMNOPQRSTUVWXYZ___^_"
 /* 0060 */ "`ABCDEFGHIJKLMNOPQRSTUVWXYZ{_}~";

#define SMAP2_START 192
#define SMAP2_END 540
	static const char smap2[] =
 /* 00C0 */ "AAAAAA_CEEEEIIII_NOOOOO_OUUUUY_SAAAAAA_CEEEEIIII_NOOOOO_OUUUUY_Y"
 /* 0100 */ "AAAAAACCCCCCCCDDDDEEEEEEEEEEGGGGGGGGHHHHIIIIIIIIII__JJKK_LLLLLLL"
 /* 0140 */ "LLLNNNNNNN__OOOOOO__RRRRRRSSSSSSSSTTTTTTUUUUUUUUUUUUWWYYYZZZZZZS"
 /* 0180 */ "BBBB__OCCDDDD_E_EFFG___IKKL_MNNOOO__PP_____TTTTUU_VYYZZ_________"
 /* 01C0 */ "_____________AAIIOOUUUUUUUUUUEAAAA__GGGGKKOOOO__J___GG__NNAA__OO"
 /* 0200 */ "AAAAEEEEIIIIOOOORRRRUUUUSSTT";

	u8 *d = buf;
	u8 *e = buf+m;
	do {
		unsigned u = *s++;
		u8 c = '_';
		if(u<SMAP1_END) c = u<SMAP1_START ? 0 : smap1[u-SMAP1_START];
		else if(u>=SMAP2_START && u<SMAP2_END) c = smap2[u-SMAP2_START];
		if(c) *d++ = c;
	} while(--n && d<e);
	return d-buf;
}

#define memeq(A,B,N) (memcmp((A),(B),(N))==0)

static int invent_name(struct file *dir, struct lookup *lup)
{
	int n, len;
	unsigned num, ndigits;
	int start = 0;

	memset(lup->sname+1, ' ', 10);

	len = lup->nlen;
	if(lup->dot > 0) {
		len = lup->dot;
		n = lup->dot+1;
		mkshort(lup->sname+8, lup->lname+n, lup->nlen-n, 3);
	}

	n = mkshort(lup->sname, lup->lname, len, 6);
	/* well need it some day:
	 * if(lup->sname[0] == 0xE5) lup->sname[0] = 5;
	 */
	num = 1;
	ndigits = 1;
	for(;;) {
		int pos = start;
		u8 slot[32];
		
		/* add suffix ~1 up to ~999 */
		{
			int p = n + ndigits;
			unsigned v = num;
			if(p>7) p = 7;
			do {
				lup->sname[p--] = v | '0';
				v >>= 8;
			} while(v);
			lup->sname[p] = '~';
		}

		for(;;) {
			int v = dir_slot(dir, pos++, slot);
			if(v<=0) return v;
			if(!memeq(slot, lup->sname, 4)) continue;
			/* first 4 letters will not change, if n>=4 */
			if(n>=4 && pos>start) start = pos;
			if(memeq(slot+4, lup->sname+4, 11-4))
				break; /* this name is used already */
		}
		
		/* next suffix */
		{
			int s=0;
			num++;
			if((num&0xFF)==10) do {
				num += 0x100-10 << s;
				s += 8;
				if(s==24) return -ENFILE;
			} while((num>>s & 0xFF) == 10);
			s >>= 3;
			if(s>=ndigits) {
				assert(s==ndigits);
				ndigits++;
			}
		}
	}
	return -ENOENT;
}

static void put16n(u8 *d, u16 *s, int n)
{
	do put16(d, *s++); while(d+=2, --n);
}

static int file_creat(struct file **fptr, struct file *dir, int pos, struct lookup *lup, u8 attr)
{
	struct file *file;
	int ret;

	file = file_alloc(dir->drive);
	if(!file) return -ENOMEM;

	if(lup->flags & LUP_INVENTSH) {
		ret = invent_name(dir, lup);
		if(ret) goto err;
	}

	{
		struct io io;
		u8 buf[21*32], *slot = buf;
		u8 csum = calc_csum(lup->sname);
		u16 *s;
		int m;
		
		m = (lup->nlen + 12)/13;
		file->nlfn = m;
		{
			int n = 13*m - lup->nlen;
			s = lup->lname + 13*m;
			if(--n>0) memset(lup->lname + lup->nlen + 1, 0xFF, 2*n);
		}
		lup->lname[lup->nlen] = 0;

		assert(m<=20);

		slot[0] = m | 0x40;
		for(;;) {
			s -= 13;
			put16n(slot+1, s, 5);
			slot[11] = 0x0F;
			slot[12] = 0;
			slot[13] = csum;
			put16n(slot+14, s+5, 6);
			put16(slot+26, 0);
			put16n(slot+28, s+11, 2);
			slot += 32;
			if(!--m) break;
			slot[0] = m;
		}

		file->mtime = fat_time;
		file->entry = pos + (slot-buf >> 5);
		init_slot(slot, lup->sname, attr, 0);

		io.data = buf;
		io.len = slot-buf+32;
		io.pos = pos<<5;
		io.op = IOOP_WRITE|IOOP_WRDIR;
		ret = file_io(dir, &io);
		if(io.len) {
			assert(ret);
			if(ret == -EFBIG)
				ret = -ENFILE; /* linux returns ENOSPC */
			goto err;
		}
		memcpy(file->dirslot, slot, 32);
	}
	file->dir = dir;
	file->clust = 0;
	list_add(&dir->childs, &file->dirlst);
	*fptr = file;
	return 0;

err:
	FREE(file);
	dput(dir->drive);
	return ret;
}
