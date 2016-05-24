/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

static unsigned fat_offset(DRIVE *dv, unsigned clust)
{
	switch(dv->flags & DRV_FAT) {
		case DRV_FAT12: return 3*clust >> 1;
		case DRV_FAT32: clust <<= 1;
		case DRV_FAT16: return clust << 1;
	}
	assert(0);
}

/* fat io */

clust_t fat_get(DRIVE *dv, clust_t clust, int must)
{
	struct buf *buf;
	unsigned pos;
	u32 val, eof;
	u8 *ptr;
	int v;

/*	if(clust<FAT_MIN_CL || clust>dv->clust_max)
		return FAT_BADNUM;*/
	assert(clust>=FAT_MIN_CL && clust<=dv->clust_max);

	pos = fat_offset(dv, clust);
	v = get_buf(dv, (pos>>SECSH) + dv->fat_start, &buf, must);
	pos &= SECSZ-1;
	if(v<=0)
		return v<0 ? v : FAT_NOTRD;

	ptr = buf->data + pos;

	if((dv->flags&DRV_FAT) == DRV_FAT32) {
		val = get32(ptr) & 0xFFFFFFF;
		eof = 0xFFFFFF8ul;
	} else {
		if(pos == SECSZ-1) {
			if(!must) return FAT_NOTRD;
			val = *ptr;
			v = get_buf(dv, buf->num+1, &buf, 1);
			if(v<1) return v;
			val |= buf->data[0] << 8;
		} else
			val = get16(ptr);

		eof = 0xFFF8;	
		if((dv->flags&DRV_FAT) == DRV_FAT12) {
			eof = 0xFF8;
			if(clust&1) val >>= 4;
			val &= 0xFFF;
		}
	}
	if(val >= eof)
		return FAT_EOF;
	if(val > dv->clust_max || val == 1)
		return FAT_BAD;
	return val;
}

static int fat_put(DRIVE *dv, clust_t clust, int val)
{
	unsigned pos;
	struct buf *buf;
	u8 *ptr;
	int v;

	assert(dv->flags&DRV_WRITE);

	pos = fat_offset(dv, clust);
	v = get_buf(dv, (pos>>SECSH) + dv->fat_start, &buf, 1);
	if(v<0) return v;
	buf_mark_dirty(buf);

	pos &= SECSZ-1;
	ptr = buf->data + pos;

	switch(dv->flags&DRV_FAT) {
		case DRV_FAT32:
			val &= 0xFFFFFFF;
			val |= get32(ptr)&0xF0000000; /* preserve high nibble */
			put32(ptr, val);
			break;
		case DRV_FAT16:
			put16(ptr, val);
			break;
		case DRV_FAT12:
			if(clust&1)
				val = *ptr&0x0F | val<<4;
			*ptr++ = val;
			val >>= 8;
			if(pos==SECSZ-1) {
				v = get_buf(dv, buf->num+1, &buf, 1);
				if(v<0) return v; /* we wrote part of entry :-( */
				buf_mark_dirty(buf);
				ptr = buf->data;
			}
			if((clust&1)==0)
				val = *ptr&0xF0 | val&0x0F;
			*ptr = val;
	}
	return 0;
}

/**/

void fat_free_chain(DRIVE *dv, clust_t c)
{
	do {
		clust_t start = c, nx;
		int num = 0;
		for(;;) {
			nx = fat_get(dv, c, 1);
			if(!clv_in_file(nx)) break;
			fat_put(dv, c, FAT_FREE);
			num++;
			if(nx != c+1) break;
			c = nx;
		}
		c = nx;
		if(num && dv->ops->free)
			dv->ops->free(dv->priv, clust_to_pos(dv, start), num<<dv->clust_sh);
	} while(clv_in_file(c) && c!=FAT_EOF);
}
		
void fat_write_chain(DRIVE *dv, clust_t pvc, clust_t c, int n, clust_t end)
{
	assert(n>0); assert(c>=FAT_MIN_CL);
	if(!pvc) goto nopv;
	do {
		fat_put(dv, pvc, c);
nopv:
		pvc = c++;
	} while(--n);
	fat_put(dv, pvc, end);
}

static int fat_space(DRIVE *dv, clust_t clust, int len)
{
	int n = need_clust(dv, len);
	assert(n>0);
	len = n;
	do {
		if(clust > dv->clust_max)
			break;
		if(fat_get(dv, clust, 1) != FAT_FREE)
			break;
		clust++;
	} while(--n);
	return len-n;
}

/* frag */

struct frag *frag_alloc()
{
	struct frag *frag = ALLOC(struct frag);
	if(frag) {
		frag->len = 0;
		frag->flags = 0;
		frag->next = 0;
	}
	return frag;
}

struct frag *frag_more(DRIVE *dv, struct frag *frag)
{
	clust_t cl, nx;

	assert(!(frag->flags & FRAG_WHOLE));
	cl = pos_to_clust(dv, frag->dpos + frag->len);
	nx = fat_get(dv, cl, 1);
	while(nx == cl+1 && nx<FAT_MAX_CL) {
		frag->len += dv->clust_sz;
		cl = nx; nx = fat_get(dv, cl, 0);
	}
	if(nx == FAT_NOTRD) {
		/* XXX what if frag not grown due to read error? */
		return frag;
	}
	frag->flags |= FRAG_WHOLE;
	if(!clv_in_file(nx)) {
		frag->flags |= FRAG_LAST | FRAG_BAD;
		return frag;
	}
	if(nx == FAT_EOF)
		frag->flags |= FRAG_LAST;
	frag->nxclust = nx;
	frag->len += dv->clust_sz;
	return frag;
}

struct frag *frag_at(DRIVE *dv, clust_t c)
{
	struct frag *frag = frag_alloc();
	if(frag) {
		frag->dpos = clust_to_pos(dv, c);
		frag->len = 0;
		frag = frag_more(dv, frag);
	}
	return frag;
}

int frag_expand(struct frag *frag, struct io *io)
{
	clust_t c = frag_last_clust(io->drive, frag);
	int l = fat_space(io->drive, c+1,
		io->len + io->pos - frag->fpos - frag->len);
	assert(!FRAG_IS(FIXED, frag));
	assert(FRAG_IS(LAST, frag));
	assert(FRAG_IS(WHOLE, frag));
	frag->len += l<<io->drive->clust_sh;
	return l;
}

static clust_t find_free(struct fat_drive *dv, clust_t start, clust_t end)
{
	clust_t c;
	for(c=start; c<=end; c++)
		if(fat_get(dv, c, 1) == FAT_FREE)
			return c;
	return 0;
}

struct frag *frag_alloc_new(struct io *io)
{
	struct frag *frag;
	clust_t c;
	int l;

	c = find_free(io->drive, io->drive->current_cl, io->drive->clust_max);
	if(!c) {
		c = find_free(io->drive, FAT_MIN_CL, io->drive->current_cl-1);
		if(!c) {
			io->err = -ENOSPC;
			return 0;
		}
	}

	frag = frag_alloc();
	if(!frag) {
		io->err = -ENOMEM;
		return frag;
	}
	frag->dpos = clust_to_pos(io->drive, c++);
	l = io->drive->clust_sz;
	if(l < io->len) {
		int n = fat_space(io->drive, c, io->len - l);
		c += n;
		l += n << io->drive->clust_sh;
	}
	io->drive->current_cl = c;
	frag->len = l;
	return frag;
}

struct frag *frag_free_chain(struct frag *frag)
{
	do {
		struct frag *f=frag->next; free(frag); frag = f;
	} while(frag);
	return frag;
}

/* io */

/* ret == 0 if short write */
int frag_io(struct frag *frag, struct io *io)
{
	u8 *ptr = io->data;
	int len = io->len;
	int pos = io->pos - frag->fpos;
	int op = io->op;
	int need, left;

	DBG_FRAG("frag_io", frag);
	DBG(" pos:%d len:%d\n", io->pos, io->len);
	assert(pos>=0);

	left = frag->len - pos;
	assert(left>0);
	if(left < len) {
		len = left;
		op &= ~IOOP_GROW;
	}

	need = len;
	pos += frag->dpos;

	if(op == IOOP_READ)
		io->err = -io->drive->ops->read(io->drive->priv, ptr, pos, &len);
	else if(io->drive->ops->write) {
		unsigned dis = op&IOOP_GROW ? left : 0;
		io->err = -io->drive->ops->write(io->drive->priv, ptr, pos, &len, dis);
		if(op&IOOP_WRDIR && len==need && dis>len) {
			dis -= len;
			io->drive->ops->write(io->drive->priv, 0, pos+len, &dis, dis);
		}
	} else {
		io->err = -EPERM;
		return 0;
	}
	io->pos += len;
	io->len -= len;
	io->data += len;
	return len==need;
}
