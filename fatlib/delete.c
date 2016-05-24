/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

/* ugly! */
inline int delete_entry(struct file *dir, int entry, int nlfn)
{
	u8 buf[1] = {0xE5};
	struct io io;
	int ret;
	io.pos = (entry - nlfn)*32;
	io.op = IOOP_WRITE;
	do {
		io.len = 1; io.data = buf;
		DBG("delete slot %d (dir clust:%X)\n", io.pos, dir->clust);
		ret = file_io(dir, &io);
		if(ret) break;
		io.pos += 32-1;
	} while(--nlfn >= 0);
	return ret;
}

int file_delete(struct file *file)
{
	DRIVE *dv = file->drive;
	int ret;

	DBG("file_delete:\n");
	if(file->drive->root == file)
		return -EBUSY; /* delete a root? */
	if(file_is_deleted(file))
		return 0;
	assert(file->dir);
	ret = will_write(dv);
	if(ret) return ret;

	ret = delete_entry(file->dir, file->entry, file->nlfn);

	file->entry = -1;
	file->nlfn = 0;

	list_del_init(&file->dirlst);
	fput(file->dir); file->dir = 0;

	return ret;
}
