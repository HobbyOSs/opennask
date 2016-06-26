/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

#ifdef __GLIBC__
static __ssize_t std_file_read(void *cookie, char *ptr, size_t len)
#else
static int std_file_read(void *cookie, char *ptr, int len)
#endif
{
	struct ofile *of = (struct ofile*)cookie;
	struct io io;
	int v, err;

	err = -EINVAL;
	if(of->pos < 0) goto reterr;
	if(of->pos+len > of->file->size) {
		len = of->file->size - of->pos;
		if(len<=0) return 0;
	}

	io.pos = of->pos;
	io.len = len;
	io.data = (u8*)ptr;
	io.op = IOOP_READ;

	err = file_io(of->file, &io);
	
	v = io.pos - of->pos;
	of->pos = io.pos;
	if(!v && err)
reterr:
		return reterr(err);
	return v;
}

#ifdef __GLIBC__
static __ssize_t std_file_write(void *cookie, const char *ptr, size_t len)
#else
static int std_file_write(void *cookie, const char *ptr, int len)
#endif
{
	struct ofile *of = (struct ofile*)cookie;
	struct io io;
	int v, err;

	if(len <= 0) return reterr(-EINVAL);
	err = will_write(of->file->drive);
	if(err) return reterr(err);

	/* XXX [glibc] do we need append flag? */

	io.pos = of->pos;
	io.len = len;
	io.data = (u8*)ptr;
	io.op = IOOP_WRITE;
	if(of->pos+len >= of->file->size) io.op |= IOOP_GROW;

	err = file_io(of->file, &io);
	if(err)
		errno = -err; /* XXX we set it even if some bytes written */
	v = io.pos - of->pos;
	of->pos = io.pos;
	if(!v) {
		assert(err);
		v = -1;
	} else {
		if(of->pos > of->file->size)
			of->file->size = of->pos;
	}
	return v;
}

inline fat_pos_t ofile_seek(struct ofile *of, fat_pos_t pos, int whence)
{
	if(whence == SEEK_CUR)
		pos += of->pos;
	else if(whence == SEEK_END)
		pos += of->file->size;
	of->pos = pos;
	return pos;
}

#if defined(__BSD__) || defined(__APPLE__)

static fpos_t std_file_seek(void *cookie, fpos_t pos, int whence)
{
	struct ofile *of = (struct ofile*)cookie;
	return ofile_seek(of, pos, whence);
}

#elif defined __GLIBC__
#if __GLIBC__<2 || __GLIBC__==2 && __GLIBC_MINOR__<2

static int std_file_seek(void *cookie, fpos_t pos, int whence)
{
	struct ofile *of = (struct ofile*)cookie;
	ofile_seek(of, pos, whence);
	return 0;
}

#else

/*#define GET_POS(P) ((fat_pos_t)(P)->__pos)
#define SET_POS(P,V) ((P)->__pos=(V))*/

static int std_file_seek(void *cookie, _IO_off64_t *posp, int whence)
{
	struct ofile *of = (struct ofile*)cookie;
	*posp = ofile_seek(of, *posp, whence);
	return 0;
}

#endif
#endif

static int std_file_close(void *cookie)
{
	struct ofile *of = (struct ofile*)cookie;
	if(libdown)
		drive_sync(of->file->drive, 0);
	fput(of->file);
	FREE(of);
	return 0;
}

FILE *fat_fopen(DRIVE *dv, const char *path, const char *mode)
{
	struct ofile *of;
	FILE *f;
	int ret;
	int perm = 0;

	{
		while(*mode) switch(*mode++) {
			case 'r': perm |= FOP_READ; break;
			case 'w': perm |= FOP_WRITE|FOP_CREAT|FOP_TRUNC; break;
			case 'a': perm |= FOP_WRITE|FOP_CREAT|FOP_APPEND; break;
			case '+': perm |= FOP_READ|FOP_WRITE; break;
			case 'R': perm |= FOP_ATTR_R; break;
			case 'H': perm |= FOP_ATTR_H; break;
			case 'S': perm |= FOP_ATTR_S; break;
			case 'A': perm |= FOP_ATTR_A; break;
		}
	}
	
	ret = -EROFS;
	if(perm&FOP_CREAT && !(dv->flags&DRV_WRITE))
		goto error;

	of = ALLOC(struct ofile);
	ret = -ENOMEM;
	if(!of) goto error;

	of->file = 0;

	ret = lookup(dv, &of->file, (char*)path,
		LOOK_FILE | (perm&FOP_CREAT ? LOOK_CREAT : 0) | perm&0x2700);
	if(ret) goto error_free;

	if(perm & FOP_TRUNC) {
		file_truncate(of->file);
		of->file->size = 0;
	}

	of->pos = perm&FOP_APPEND ? of->file->size : 0;
	of->perm = perm;

#if defined(__BSD__) || defined(__APPLE__)
	f = funopen(of,
		perm&FOP_READ ? std_file_read : 0,
		perm&FOP_WRITE ? std_file_write : 0,
		std_file_seek, std_file_close);
#elif defined __GLIBC__
	{
		char mbuf[4];
		if(perm&FOP_TRUNC) mbuf[0] = 'w';
		else if(perm&FOP_APPEND) mbuf[0] = 'a';
		else mbuf[0] = 'r';
		mbuf[1] = 0;
		if((perm&(FOP_READ|FOP_WRITE))==(FOP_READ|FOP_WRITE)) {
			mbuf[1] = '+';
			mbuf[2] = 0;
		}
		f = fopencookie(of, mbuf, (cookie_io_functions_t){
			write: std_file_write,
			read: std_file_read,
			seek: std_file_seek,
			close: std_file_close,
		});
	}
#else
#error funopen() equivalent not known
#endif
	if(!f) {
		fput(of->file);
		FREE(of);
	}
	return f;

error_free:
	FREE(of);
error:
	errno = -ret;
	return 0;
}
