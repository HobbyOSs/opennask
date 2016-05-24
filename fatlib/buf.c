/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

/* small buffer cache used for FAT */

inline int buf_in_fat(struct buf *b)
{
	DRIVE *dv = b->drive;
	int n = b->num - dv->fat_start;
	return n>=0 && n<dv->fat_nsec;
}

int read_buf(struct buf *buf)
{
	DRIVE *dv = buf->drive;
	unsigned num = buf->num;
	int v, n=1, len;

	if(buf_in_fat(buf)) {
		n = dv->fat_num;
		num += dv->fat_use*dv->fat_nsec;
	}
again:
	len = SECSZ;
	v = -dv->ops->read(dv->priv, buf->data, num<<9, &len);
	assert(v || len==SECSZ);
	if(v) {
		if(--n) {
			dv->fat_use++;
			num += dv->fat_nsec;
			if(dv->fat_use >= dv->fat_num) {
				dv->fat_use = 0;
				num -= dv->fat_num*dv->fat_nsec;
			}
			goto again;
		}
		buf->bad = 1;
	}
	return v;
}

int write_buf(struct buf *buf)
{
	DRIVE *dv = buf->drive;
	unsigned num = buf->num;
	int n = buf_in_fat(buf) ? dv->fat_num : 1;
	int v;

	for(;;) {
		int len = SECSZ;
		v = -dv->ops->write(dv->priv, buf->data, num<<9, &len, 0);
		assert(v || len==SECSZ);
		if(!--n) break;
		num += dv->fat_nsec;
	}
	
	list_del_init(&buf->dirty);
	return v;
}

static int bufn = 0;
static list_t bhash[16];
static struct buf bufs[16];
static u8 bdata[16][SECSZ];

__attribute__((constructor)) void buf_init()
{
	int n;
	for(n=0; n<elemof(bhash); n++)
		list_init(&bhash[n]);
	for(n=0; n<elemof(bufs); n++) {
		bufs[n].data = bdata[n];
		list_init(&bufs[n].dirty);
		list_init(&bufs[n].hash);
	}
}

inline list_t *slot(DRIVE *dv, unsigned num)
{
	num ^= (unsigned long)dv;
	num ^= num>>4;
	num ^= num>>8;
	return &bhash[num & elemof(bhash)-1];
}

inline void bhash_add(struct buf *buf)
{
	list_del_add(slot(buf->drive, buf->num), &buf->hash);
}

inline void bhash_del(struct buf *buf)
{
	list_del_init(&buf->hash);
}

static struct buf *find_buf(DRIVE *dv, unsigned num)
{
	list_t *h = slot(dv, num);
	list_t *l;
	for(l=h->next; l!=h; l=l->next) {
		struct buf *b = list_entry(l, struct buf, hash);
		if(b->drive == dv && b->num == num)
			return b;
	}
	return 0;
}

/* 0 not found, 1 found, <0 error */
int get_buf(DRIVE *dv, unsigned num, struct buf **bufp, int must)
{
	struct buf *buf = find_buf(dv, num);

	if(!buf) {
		int v;

		if(!must)
			return 0;

		buf = &bufs[bufn];
		bufn = bufn+1 & elemof(bufs)-1;

		if(!list_empty(&buf->dirty))
			write_buf(buf);

		assert(list_empty(&buf->dirty));
		buf->drive = dv;
		buf->num = num;
		buf->bad = 0;
		bhash_add(buf);

		v = read_buf(buf);
		if(v) return v;
	}
	*bufp = buf;
	return 1;
}

/* m!=0 remove */
inline void buf_sync(struct buf *buf, int m)
{
	if(!list_empty(&buf->dirty))
		write_buf(buf);
	if(m) {
		bhash_del(buf);
		buf->drive = 0;
	}
}

void sync_drive_bufs(DRIVE *dv, int m)
{
	int n;
	for(n=0; n<elemof(bufs); n++) if(bufs[n].drive == dv)
		buf_sync(&bufs[n], m);
}
