/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

/*static u8 *get_fsinfosec(DRIVE *dv, int sec)
{
	struct buf *buf;
	if(!sec) return 0;
	if(get_buf(dv, sec<<(dv->sec_sh-SECSH), &buf, 1)!=1) return 0;
	if(get32(buf->data+0) != 0x41615252) return 0;
	if(get32(buf->data+484) != 0x61417272) return 0;
	if(get32(buf->data+508) != 0xAA550000) return 0;
	return buf->data;
}*/

/* flags: 1 - need free sec, 2 - need bad sec */
int fat_dstat(DRIVE *dv, struct fat_dstat *st, int flags)
{
	unsigned clsh = dv->clust_sh - dv->sec_sh;
	struct buf *boot;
	int err;
	u8 *p;

	if(!(dv->flags & DRV_OPEN)) /* XXX ? */
		return reterr(-ENODEV);

	err = get_buf(dv, 0, &boot, 1);
	if(err<0)
		return reterr(err);
	memcpy(st->oemid, boot->data+3, 8);
	st->serial = 0;
	st->label[0] = 0;
	p = boot->data + ((dv->flags&DRV_FAT) != DRV_FAT32 ? 0x26 : 0x42);
	if(*p == 0x29 || *p == 0x28 /*?*/) {
		int l;
		st->serial = get32(p+1);
		p += 5;
		for(l=11; l>0; l--)
			if(p[l-1]!=' ') break;
		memcpy(st->label, p, l);
		st->label[l] = 0;
	}

	st->sector_sh = dv->sec_sh;
	st->cluster_sh = clsh;
	st->size = dv->totsec;
	st->capacity = (dv->clust_max-1)<<clsh;

	st->free = 0;
	st->bad = 0;

/*	if(flags==1 && (dv->flags&DRV_FAT)==DRV_FAT32) {
		int sec = get32(boot->data+48);
		u8 *b = get_fsinfosec(dv, sec);
		if(b) {
			st->free = get32(b+488); 
			flags = 0;
		}
	}*/
	if(flags & 3) {
		int n, m=dv->clust_max;
		unsigned s=0, b=0;
		for(n=2; n<=m; n++) {
			int v = fat_get(dv, n, 1);
			s += (v == FAT_FREE);
			b += (v == FAT_BAD);
		}
		st->free = s<<clsh;
		st->bad = b<<clsh;
	}

	switch(dv->flags & DRV_FAT) {
		case DRV_FAT12: st->fat_type = fat_fat12; break;
		case DRV_FAT16: st->fat_type = fat_fat16; break;
		case DRV_FAT32: st->fat_type = fat_fat32; break;
	}
	return 0;
}
