/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

char *fat__put_utf8_p(char *d, char *e, unsigned u)
{
	if(u<0x800) {
		if(d >= e) return 0;
		d[0] = 0xC0 | u>>6;
	} else {
		d++;
		if(d >= e) return 0;
		d[-1] = 0xE0 | u>>12;
		d[0] = 0x80 | u>>6 & 0x3F;
	}
	d[1] = 0x80 | u & 0x3F;
	return d + 2;
}

FDIR *fat_opendir(DRIVE *dv, const char *name)
{
	FDIR *dir = ALLOC(FDIR);
	int err = -ENOMEM;
	if(dir) {
		dir->pos = -1;
		dir->file = 0;
		err = lookup(dv, &dir->file, (char*)name, LOOK_DIR);
		if(!err) return dir;
		FREE(dir);
	}
	errno = -err;
	return 0;
}

void fat_closedir(FDIR *dir)
{
	fput(dir->file);
	FREE(dir);
}

static void sname_to_uni(DRIVE *dv, struct rd *rd)
{
	u16 *d = rd->name;
	int n, nspac;
	u8 c;

	n = 0; nspac = 0;
	if(rd->slot[11]&0x10) { /* directiories */
		c = rd->slot[0];
		goto putchar;
	}

	for(; n<11; n++) {
		if(n==8) {
			*d++ = '.';
			nspac = 0;
		}
		c = rd->slot[n];
		if(c==' ') {
			nspac++;
			continue;
		}
		if(nspac) do *d++ = ' '; while(--nspac);
		if(c<0x80) {
			if(c>='A' && c<='Z') c+=32;
		} else c = fat_cp_lower(dv, c);
putchar:
		*d++ = c<=0x80 ? c : fat_cp_to_u(dv, c);
	}
	if(d[-1] == '.') d--;
	*d = 0; /* ??? */
	rd->nlen = d - rd->name;
}

int fat_readdir(FDIR *dir, struct fat_dirent *de)
{
	struct rd rd;
	int v;

	rd.dir = dir->file;
	rd.pos = dir->pos;

again:
	v = read_entry(&rd);
	if(v<=0) return v;

	if(rd.slot[11] & 0x08) /* label */
		goto again;

	memcpy(de->sname, rd.slot, 11);
	de->attr = rd.slot[11];

	if(!rd.nlfn)
		sname_to_uni(dir->file->drive, &rd);

	{
		u16 *s = rd.name;
		u8 *d = de->name;
		int n = rd.nlen;
		do {
			d = fat_put_utf8(d, endof(de->name)-1, *s++);
			if(!d) goto again; /* ignore name if too long */
		} while(--n);
		*d = 0;
		de->nlen = (char*)d-de->name;
	}

	de->size = get32(rd.slot+0x1C);
	de->cluster = get16(rd.slot+0x1A) | get16(rd.slot+0x14)<<16;
	memcpy(de->mtime, rd.slot+0x16, 4);
	memcpy(de->xtime, rd.slot+0x0E, 4);

	dir->pos = rd.pos;
	return 1;
}
