/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

__attribute__((weak))
unsigned fat_cp_to_u(DRIVE *dv, unsigned char c)
{
	return nocp_cp_to_u(c);
}

__attribute__((weak))
unsigned char fat_cp_lower(DRIVE *dv, unsigned char c)
{
	return nocp_cp_lower(c);
}

__attribute__((weak))
unsigned char fat_u_to_cp(DRIVE *dv, unsigned u)
{
	return nocp_u_to_cp(u);
}
