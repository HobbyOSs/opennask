/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

int fat_mkdir(DRIVE *dv, const char *path)
{
	struct file *dir = 0;
	struct io io;
	u8 buf[64];
	int ret;

	ret = lookup(dv, &dir, path, LOOK_DIR|LOOK_CREAT|LOOK_EXCL);
	if(ret) return reterr(ret);

	io.pos = 0;
	io.data = buf;
	io.len = sizeof buf;
	io.op = IOOP_WRITE|IOOP_WRDIR;

	init_slot(buf, ".          ", 0x10, 0);
	init_slot(buf+32, "..         ", 0x10, dir->dir ? dir->dir->clust : 0);

	ret = file_io(dir, &io);
	if(io.len)
		file_delete(dir);

	fput(dir);
	return result(ret);
}
