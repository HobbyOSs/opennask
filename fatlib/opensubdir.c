/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

FDIR *fat_opensubdir(FDIR *parent, const char *name)
{
	FDIR *dir = ALLOC(FDIR);
	int err = -ENOMEM;
	if(dir) {
		dir->pos = -1;
		dir->file = fget(parent->file);
		err = lookup(parent->file->drive, &dir->file, (char*)name, LOOK_DIR);
		if(!err) return dir;
		FREE(dir);
	}
	errno = -err;
	return 0;
}
