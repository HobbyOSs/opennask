/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#define _GNU_SOURCE
#include <sys/types.h>
#ifdef __linux__
#include <stdint.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <assert.h>

#pragma clang diagnostic ignored "-Wshift-op-parentheses"
#pragma clang diagnostic ignored "-Wlogical-op-parentheses"
#pragma clang diagnostic ignored "-Wbitwise-op-parentheses"
#pragma clang diagnostic ignored "-Wpointer-sign"
#pragma clang diagnostic ignored "-Wgnu-designator"

#if defined __i386__ || defined __x86_64__
#define UNALIGNED 1
#endif

#ifndef __BSD__
#if defined __FreeBSD__ || defined __OpenBSD__ || defined __NetBSD__
#define __BSD__ 1
#endif
#endif

#define FAT_IMPL

#define DU(N) typedef uint ## N ## _t u ## N
DU(8); DU(16); DU(32);
#undef DU

struct fat_cp {
	struct cp_assoc *rmap;
	u16 map[128];
	u8 lower[128];
};

#include "fatlib.h"
#include "list.h"

#define inline static inline

#if 0
#define DBG printf
#else
#define DBG(A...)
#endif
#define DBG_FRAG(T,F) DBG(T " frag {%X fpos:%d dpos:%d len:%d}\n", (F)->flags, (F)->fpos, (F)->dpos, (F)->len);

#define elemof(T) (sizeof T/sizeof*T)
#define endof(T) (T + elemof(T))

typedef long fat_pos_t;
typedef unsigned clust_t;

#define ALLOCN(T,N) ((T*)malloc(sizeof(T)*(N)))
#define ALLOC(T) ALLOCN(T,1)
#define FREE(P) free(P)

inline u16 get16(u8 *p)
{
#if !UNALIGNED
	return p[0] | (u16)p[1]<<8;
#else
	return *(u16*)p;
#endif
}

inline u32 get32(u8 *p)
{
#if !UNALIGNED
	return p[0] | (u32)p[1]<<8 | (u32)p[2]<<16 | (u32)p[3]<<24;
#else
	return *(u32*)p;
#endif
}

inline void put16(u8 *p, u16 v)
{
#if !UNALIGNED
	p[0] = v; p[1] = v>>8;
#else
	*(volatile u16*)p = v;
#endif
}

inline void put32(u8 *p, u32 v)
{
#if !UNALIGNED
	p[0] = v; p[1] = v>>8; p[2] = v>>16; p[3] = v>>24;
#else
	*(volatile u32*)p = v;
#endif
}

inline int reterr(int err) {errno=-err; return -1;}
inline int result(int val) {return val<0 ? reterr(val) : val;}
extern int libdown;

struct fat_drive {
	list_t list; /* list of all open drives */
	list_t files_dirty;
	list_t bufs_dirty;
	int count;
	unsigned flags;
#define DRV_FAT12 0
#define DRV_FAT16 1
#define DRV_FAT32 2
#define DRV_FAT 3
#define DRV_WRITE 8
#define DRV_OPEN 16
	struct fat_dops *ops;
	void *priv;

	int clust_sh, clust_sz;
	int clust_max;
	int base;

	u8 fat_num, fat_use;
	unsigned fat_start, fat_nsec; /* sec */
	unsigned root_start;
	int root_ent;
	clust_t root_clust;

	struct file *root;
	fat_cp_t cp;
	clust_t current_cl;
	
	unsigned totsec;
	unsigned char sec_sh;
};

void drive_sync(DRIVE *dv, int m);
void drive_close(DRIVE *dv);

inline int will_write(DRIVE *dv)
{
	return dv->flags&DRV_WRITE ? 0 : -EROFS;
}

inline void dget(DRIVE *dv)
{
	dv->count++;
}

inline void dput(DRIVE *dv)
{
	if(!--dv->count)
		drive_close(dv);
}

inline clust_t pos_to_clust(DRIVE *dv, fat_pos_t pos)
{
	return pos-dv->base >> dv->clust_sh;
}

inline fat_pos_t clust_to_pos(DRIVE *dv, clust_t cl)
{
	fat_pos_t pos = dv->base + (cl<<dv->clust_sh);
	assert(pos_to_clust(dv, pos) == cl); /* avoid overflow */
	return pos;
}

inline unsigned need_clust(DRIVE *dv, fat_pos_t size)
{
	assert(size>0);
	return (size-1 >> dv->clust_sh) + 1;
}

#define SECSZ 512	/* internal sector unit */
#define SECSH 9
struct buf {
	list_t hash;	/* on bhash[] */
	list_t dirty;	/* on drive->bufs_dirty */
	DRIVE *drive;
	unsigned num;
	u8 *data;
	u8 bad:1;
};

int get_buf(DRIVE *dv, unsigned num, struct buf **bufp, int must);
void sync_drive_bufs(DRIVE *dv, int m);

inline void buf_mark_dirty(struct buf *buf)
{
	if(list_empty(&buf->dirty))
		list_add(&buf->drive->bufs_dirty, &buf->dirty);
}

#define FAT_FREE	0x00000000
#define FAT_MIN_CL	0x00000002
#define FAT_MAX_CL	0x0FFFFFEF
#define FAT_EOF		0x0FFFFFFF
#define FAT_BAD		0x0FFFFFF7
#define FAT_NOTRD	0x10000000	/* not read */
#define FAT_BADNUM	0x10000001

inline int clv_is_bad(clust_t c) {return c!=FAT_EOF && c>FAT_MAX_CL;}
inline int clv_in_file(clust_t c) {return c!=FAT_FREE && c<=FAT_MAX_CL || c==FAT_EOF;}

clust_t fat_get(DRIVE *dv, clust_t clust, int must);
void fat_write_chain(DRIVE*, clust_t pvc, clust_t c, int n, clust_t end);
void fat_free_chain(DRIVE *dv, clust_t c);
struct io {
	enum {
		IOOP_READ  = 0,
		IOOP_WRITE = 1,
		IOOP_GROW  = 2,
		IOOP_WRDIR = 4
	} op;
	DRIVE *drive;
	u8 *data;
	int pos, len, err;
};

struct frag {
	unsigned flags;
#define FRAG_WHOLE 1	/* not part of fragment */
#define FRAG_LAST 2	/* no more fragments */
#define FRAG_BAD 8	/* can't grow because of error */
#define FRAG_FIXED 16	/* can't be changed */
	fat_pos_t dpos, fpos, len;
	int nxclust; /* only if WHOLE and !LAST */
	struct frag *next;
};

#define FRAG_IS(A,F) ((F)->flags&FRAG_##A)
inline int frag_fend(struct frag *frag) {return frag->fpos + frag->len;}

inline clust_t frag_first_clust(DRIVE *dv, struct frag *frag)
{
	return pos_to_clust(dv, frag->dpos);
}

inline clust_t frag_last_clust(DRIVE *dv, struct frag *frag)
{
	return pos_to_clust(dv, frag->dpos+frag->len-1);
}

struct frag *frag_alloc();
struct frag *frag_at(DRIVE*, clust_t);
struct frag *frag_more(DRIVE *dv, struct frag *frag);
struct frag *frag_alloc_new(struct io *io);
int frag_expand(struct frag *frag, struct io *io);
struct frag *frag_free_chain(struct frag *frag); /* ret: NULL */
int frag_io(struct frag *frag, struct io *io);

struct file {
	list_t dirlst; /* entry */
	list_t childs; /* head */
	list_t dirty; /* dirty list */
	DRIVE *drive;
	int count;

	struct file *dir;
	int entry, nlfn;
	u8 dirslot[32];
	time_t mtime;

	clust_t clust;
	unsigned size;

	struct frag *begin, *end;
};

struct file *file_alloc(DRIVE *drive);
void file_free(struct file *file);

inline int file_is_dirty(struct file *file) {return !list_empty(&file->dirty);}
inline int file_is_deleted(struct file *file) {return file->entry<0;}

inline void file_modified(struct file *file)
{
	if(list_empty(&file->dirty))
		list_add(&file->drive->files_dirty, &file->dirty);
	file->mtime = fat_time;
}

inline struct file *fget(struct file *file)
{
	DBG("fget %p \"%.11s\" #%d\n", file, file->dirslot, file->count);
	file->count++;
	return file;
}

inline void fput(struct file *file)
{
	DBG("fput %p \"%.11s\" #%d\n", file, file->dirslot, file->count);
	if(!--file->count)
		file_free(file);
}

inline clust_t file_first_cluster(struct file *file) {
	return file->clust>1 && file->clust<=file->drive->clust_max
		? file->clust : 0;
}

struct file *drive_get_root(DRIVE *dv);

int file_io(struct file *file, struct io *io);
int file_write_meta(struct file *file);
int file_delete(struct file *file);
int file_truncate(struct file *file);

struct ofile {
	struct file *file;
#define FOP_READ 1
#define FOP_WRITE 2
#define	FOP_CREAT 4
#define FOP_APPEND 8
#define FOP_TRUNC 16
#define FOP_ATTR_R 0x0100
#define FOP_ATTR_H 0x0200
#define FOP_ATTR_S 0x0400
#define FOP_ATTR_A 0x2000
	int perm;
	int pos;
};

struct rd {
	struct file *dir;
	int pos;

	int empt_pos, empt_need;

	int nlen, nlfn;
	u8 slot[32];
	u16 name[260];
};

int dir_slot(struct file *dir, int pos, u8 slot[32]);
void init_slot(u8 slot[32], const u8 *sname, u8 attr, int clust);

/* ret: <0 err, 0 eof, 1 read */
int read_entry(struct rd *rd);

struct fat_dir {
	struct file *file;
	int pos; /* last read entry number */
};

#define LOOK_FILE 1
#define LOOK_DIR 2
#define LOOK_LABEL 4
#define LOOK_CREAT 8
#define LOOK_EXCL 16
int lookup_one(struct file **fptr, const char *name, int nlen, int flags);
int lookup(DRIVE *dv, struct file **fptr, const char *path, int flags);

inline unsigned nocp_cp_to_u(unsigned char c) {
	return 0xF000 + (u8)c;
}

inline unsigned char nocp_cp_lower(unsigned char c) {
	return c;
}

inline unsigned char nocp_u_to_cp(unsigned u) {
	return u >= 0xF080 && u < 0xF100 ? u : 0;
}

#ifdef __FreeBSD__
#define OFILE_FROM_FILE(F) ((struct ofile*)(F)->_cookie)
#elif defined __GLIBC__
#define OFILE_FROM_FILE(F) ((struct ofile*)((struct _IO_cookie_file*)(F))->__cookie)
#endif
