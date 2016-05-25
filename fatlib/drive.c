/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

static list_t drives;
int libdown = 0;
time_t fat_time;

inline int drive_log2(unsigned v)
{
	if(v & v-1)
		return -EINVAL;
	return ffs(v)-1;
}

static int drive_open(DRIVE *dv, u8*);

void drive_sync(DRIVE *dv, int m)
{
	/* single directory may be synced many times, it can be dirtied again
	   when its child is synced :-(  (introduce dirty_count field?) */
	while(!list_empty(&dv->files_dirty)) {
		struct file *file = list_entry(dv->files_dirty.next, struct file, dirty);
		file_write_meta(file);
		assert(!file_is_dirty(file));
	}
	/* XXX clear dirty bit in FAT */
	sync_drive_bufs(dv, m);
	if(dv->ops->sync)
		dv->ops->sync(dv->priv);
}

static void theend()
{
	list_t *l;
	libdown = 1;
	/* XXX if drive contains FILEs open, we can delay syncing
	 * until stdio closes them */
	for(l=drives.next; l!=&drives; l=l->next)
		drive_sync(list_entry(l, DRIVE, list), 0);
}

void drive_close(DRIVE *dv)
{
	DBG("drive_close\n");
	list_del(&dv->list);
	drive_sync(dv, 1);
	if(dv->ops->close)
		dv->ops->close(dv->priv);
	FREE(dv);
}

DRIVE *fat_open_virt(struct fat_dops *dops, void *priv, int rw)
{
	DRIVE *dv = ALLOC(DRIVE);
	u8 buf[512];
	int v;

	if(!dv) return dv;

	dv->priv = priv;
	dv->ops = dops;
	if(!dops->write) rw = 0;
	dv->flags = rw&1 ? DRV_WRITE : 0;

	list_init(&dv->files_dirty);
	list_init(&dv->bufs_dirty);
	dv->root = 0;
	dv->count = 1;

	v = sizeof buf;
	v = -dops->read(priv, buf, 0, &v);
	if(v) goto error;

	v = drive_open(dv, buf);
	if(v) goto error;

	if(!drives.next) {
		list_init(&drives);
		atexit(theend);
	}
	list_add(&drives, &dv->list);

	dv->current_cl = FAT_MIN_CL;
	dv->cp = 0;
	dv->flags |= DRV_OPEN;
	return dv;

error:
	FREE(dv);
	errno = -v;
	return 0;
}

void fat_drive_close(DRIVE *dv)
{
	dput(dv);
}

static int drive_open(DRIVE *dv, u8 *buf)
{
	int fat;
	int secsh, clssh;
	unsigned totsec, ressec, fatsec, rootsec;
	unsigned fatent, nclust;

	/* F6F6F6... - just formatted ?? */
	/*
	 * EB rr 90	0x22 <= rr <= 0x80
	 * E9 rr 00	0x21 <= rr
	 * E9 rr 01	rr <= 0xF9
	 * 60 ?? ??
	 */
	if(buf[0]!=0xEB && buf[0]!=0xE9 && buf[0]!=0x60)
		return -EINVAL;
	if(get16(buf+0x1FE)!=0xAA55)
		return -EINVAL;

	secsh = drive_log2(get16(buf+11));
	if(secsh<9 || secsh>12)
		return -EINVAL;

	totsec = get16(buf+19);
	if(!totsec) {
		totsec = get32(buf+32);
		if(!totsec) return -EINVAL;
	}

#if 1
	if((fat_pos_t)1 << sizeof(fat_pos_t)*8-1-secsh <= totsec)
		return -EFBIG;
#endif

	fatsec = get16(buf+22);
	fat = DRV_FAT16;
	if(!fatsec) {
		fat = DRV_FAT32;
		fatsec = get32(buf+36);
		if(!fatsec) return -EINVAL;
		dv->root_ent = rootsec = 0;
	} else {
		unsigned v = get16(buf+17);
		if(!v) return -EINVAL;
		dv->root_ent = v;
		rootsec = (v-1 >> (secsh-5)) + 1;
	}

	clssh = drive_log2(buf[13]);
	if(clssh<0)
		return -EINVAL;

	ressec = get16(buf+14);
	if(!ressec)
		return -EINVAL;
	if(totsec <= ressec + buf[16]*fatsec + rootsec)
		return -EINVAL;
	nclust = (totsec - (ressec + buf[16]*fatsec + rootsec)) >> clssh;
	if(!nclust)
		return -EINVAL;

	if(fat != DRV_FAT32) {
		if(nclust>=65525) return -EINVAL;
		fatent = fatsec << secsh-1;
		if(nclust<4085) {
			fat = DRV_FAT12;
			fatent = (fatsec << secsh+1) / 3;
		}
		dv->root_clust = 0;
	} else {
		if(nclust<65525 || nclust+1>FAT_MAX_CL) return -EINVAL;
		fatent = fatsec<<(secsh-2);
		dv->root_clust = get32(buf+0x2C);
		if(dv->root_clust<2 || dv->root_clust>nclust+1)
			return -EINVAL;
	}

	if(fatent < nclust+2)
		return -EINVAL;

	dv->flags |= fat;
	dv->clust_sh = secsh + clssh;
	dv->clust_sz = 1<<dv->clust_sh;
	dv->clust_max = nclust+1;
	dv->fat_num = buf[16];
	if(dv->fat_num < 1)
		return -EINVAL;
	dv->fat_use = 0;
	dv->fat_start = ressec<<(secsh-SECSH);
	dv->fat_nsec = fatsec<<(secsh-SECSH);

	dv->root_start = dv->fat_start + dv->fat_num*dv->fat_nsec;

	dv->base = (dv->root_start<<SECSH) + (rootsec<<secsh) - (2<<dv->clust_sh);

	/* for dstat only */

	dv->totsec = totsec;
	dv->sec_sh = secsh;

	return 0;
}
