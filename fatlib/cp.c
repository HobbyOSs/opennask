/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"

unsigned fat_cp_to_u(DRIVE *dv, unsigned char c)
{
	fat_cp_t cp = dv->cp;
	return cp ? cp->map[c-0x80] : nocp_cp_to_u(c);
}

unsigned char fat_cp_lower(DRIVE *dv, unsigned char c)
{
	fat_cp_t cp = dv->cp;
	return cp ? cp->lower[c-0x80] : nocp_cp_lower(c);
}

// UNICODE -> CP

struct cp_assoc {
	u16 u;
	u8 c;
};

/* ret: 0x80..0xFF ok, 0 not found */
unsigned char fat_u_to_cp(DRIVE *dv, unsigned u)
{
	struct cp_assoc *r;
	int b = 0, w = 64;
	if(!dv->cp) return nocp_u_to_cp(u);
	r = dv->cp->rmap;
	do {
		if(u >= r[b+w].u)
			b += w;
		w >>= 1;
	} while(w);
	return r[b].u == u ? r[b].c : 0;
}

#if 0
static int cmp(const void *va, const void *vb)
{
	return (int)((struct cp_assoc*)va)->u - (int)((struct cp_assoc*)vb)->u;
}
#endif

int fat_set_cp(DRIVE *dv, fat_cp_t cp)
{
	int n;

	dv->cp = cp;
	if(!cp || cp->rmap)
		return 0;
	cp->rmap = ALLOCN(struct cp_assoc, 128);
	if(!cp->rmap) {
		dv->cp = 0;
		return -1;
	}
#if 0
	for(n=0;n<128;n++) {
		cp->rmap[n].u = (u32)cp->map[n];
		cp->rmap[n].c = n | 128;
	}
	qsort(cp->rmap, 128, sizeof cp->rmap[0], cmp);
#else
	for(n=0;n<128;n++) {
		int u = cp->map[n];
		int i, j;
		for(i=0;i<n;i++) if(cp->rmap[i].u < u) break;
		for(j=n;j>i;j--) cp->rmap[j] = cp->rmap[j-1];
		cp->rmap[i].u = u;
		cp->rmap[i].c = n | 128;
	}
#endif
	return 0;
}
