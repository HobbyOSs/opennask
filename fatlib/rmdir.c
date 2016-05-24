/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

/* 0:empt, -ENOTEMPTY:not empty, <0:err */
static int dir_check_empty(struct file *dir)
{
	int pos;
	assert(dir->dir);

	if(!list_empty(&dir->childs))
		return -ENOTEMPTY;

	pos = 2;
	for(;;) {
		u8 slot[32];
		int v = dir_slot(dir, pos++, slot);
		if(v<1) return v;
		v = 0;
		if(slot[0]==0) return v;
		if(slot[0]!=0xE5 && slot[11]!=0x0F)
			return -ENOTEMPTY;
	}
}

int fat_rmdir(DRIVE *dv, const char *path)
{
	struct file *dir = 0;
	int ret;
	ret = will_write(dv);
	if(ret) goto ret;
	ret = lookup(dv, &dir, path, LOOK_DIR);
	if(ret) goto ret;
	ret = dir_check_empty(dir);
	if(!ret)
		ret = file_delete(dir);
	fput(dir);
ret:
	return result(ret);
}
