/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

void fat_rewinddir(FDIR *dir)
{
	dir->pos = -1;
}

int fat_telldir(FDIR *dir)
{
	return dir->pos;
}

void fat_seekdir(FDIR *dir, int pos)
{
	dir->pos = pos;
}
