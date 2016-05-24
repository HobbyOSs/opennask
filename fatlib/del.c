/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

int fat_delete(DRIVE *dv, const char *path)
{
	struct file *file = 0;
	int ret;

	ret = lookup(dv, &file, path, LOOK_FILE);
	if(!ret) {
		assert(file);
		ret = file_delete(file);
		fput(file);
	}
	return result(ret);
}
