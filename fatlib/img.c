/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"
#include <fcntl.h>
#ifndef _WIN32
  #include <unistd.h>
  #include <sys/file.h>
#endif

struct img_priv {
	int fd;
	unsigned pos;
};

const static u8 zeros[512];

static int zero(int fd, int len)
{
	int n = len % sizeof zeros;
	int l = 0;
	if(n) {
		l = write(fd, zeros, n);
		if(l<n) return l;
	}
	n = len / sizeof zeros;
	if(n) do {
		int v = write(fd, zeros, sizeof zeros);
		if(v>0) l+=v;
		if(v<0) break;
	} while(--n);
	return l;
}

static int img_io(void *priv, char *buf, off_t pos, int *lenp, int rw)
{
	struct img_priv *im = (struct img_priv*)priv;
	int len = *lenp, v;

	if(im->pos != pos) {
		lseek(im->fd, pos, SEEK_SET);
		im->pos = pos;
	}
	if(rw==0)
		v = read(im->fd, buf, len);
	else if(buf)
		v = write(im->fd, buf, len);
	else
		v = zero(im->fd, len);
	if(v>0) {
		im->pos += v;
		*lenp = v;
	} else {
		im->pos = -1;
		*lenp = 0;
	}
	if(v<0)
		v = errno;
	else if(v<len)
		v = ENXIO; /* image truncated */
	else
		v = 0;
	return v;
}

static int img_read(void *priv, char *buf, long pos, int *lenp)
{
	return img_io(priv, buf, pos, lenp, 0);
}

static int img_write(void *priv, const char *buf, long pos, int *lenp, int dis)
{
	dis=dis;
	return img_io(priv, (char*)buf, pos, lenp, 1);
}

static void img_close(void *priv)
{
	struct img_priv *im = (struct img_priv*)priv;
	close(im->fd);
	FREE(im);
}

static struct fat_dops img_dops = {
	read: img_read,
	write: img_write,
	close: img_close,
};

DRIVE *fat_open_image(const char *path, int rw)
{
	struct img_priv *im;
	DRIVE *dv = 0;
	int fd;
#ifdef O_EXLOCK
	fd = open(path, rw ? O_RDWR|O_EXLOCK : O_RDONLY|O_SHLOCK);
	if(fd<0) goto ret;
#else
	fd = open(path, rw ? O_RDWR : O_RDONLY);
	if(fd<0) goto ret;
	{
		int v = flock(fd, (rw ? LOCK_EX : LOCK_SH) | LOCK_NB);
		if(v<0) {
			close(fd);
			goto ret;
		}
	}
#endif
	im = ALLOC(struct img_priv);
	if(!im) {
		close(fd);
		return 0;
	}
	im->fd = fd;
	im->pos = 0;

	dv = fat_open_virt(&img_dops, im, rw);
	if(!dv)
		img_close(im);
ret:
	return dv;
}
