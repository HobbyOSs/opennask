/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include <time.h>
#include <stdio.h>

#ifdef __linux__
#define FLOPPY_DEFAULT "/dev/fd0"
#endif

struct fat_dops {
/*
 * return value:
 *   0		success (*len bytes read/written)
 *   Exxx	errno error (*len updated to actual byte count)
 * error should not be returned if *len remains unchanged
 * *len will never be <= 0 on entry
 * if on write buf==NULL region should be zeroed
 * information in pos+len..pos+discard-1 is not important for fs
 */
	int (*read)(void *priv, char *buf, long pos, int *len);
	int (*write)(void *priv, const char *buf, long pos, int *len, int discard);
	void (*free)(void *priv, long pos, int len); /* region no longer used */
	int (*sync)(void *priv);
	void (*close)(void *priv);
};

typedef struct fat_drive DRIVE;
typedef struct fat_dir FDIR;

/* rw should be: 0 readonly, 1 read-write, bits>0 reserved */
DRIVE *fat_open_image(const char *path, int rw);
DRIVE *fat_open_virt(struct fat_dops *dops, void *priv, int rw);
void fat_drive_close(DRIVE *dv);

/**
 * fat_fopen - open stdio FILE
 * @drive: drive
 * @path: path to file
 * @mode: fopen() flags
 *
 * flags:
 *	r    open for reading
 *	w    truncate or create for writing
 *	a    open or create, write at end of file
 *	r+   open for reading and writing
 *	w+   truncate or create for reading and writing
 *	a+   open or create, read-write at end of file
 *	RHSA attributes for created file
 */
FILE *fat_fopen(DRIVE *drive, const char *path, const char *mode);
int fat_delete(DRIVE *dv, const char *path);

struct fat_dirent {
	int nlen;
	unsigned cluster;
	unsigned size;
	char name[260];		/* long name (utf-8) */
	char sname[11];
	unsigned char attr;
	unsigned char mtime[4]; /* dos format */
	unsigned char xtime[4]; /* dos format */
};
#define FAT_GET_YEAR(T) (((T)[3]>>1&0x7F)+1980)
#define FAT_GET_MONTH(T) ((T)[3]<<3&8|(T)[2]>>5&7)
#define FAT_GET_DAY(T) ((T)[2]&0x1F)
#define FAT_GET_HOUR(T) ((T)[1]>>3&0x1F)
#define FAT_GET_MIN(T) ((T)[1]<<3&0x38|*(T)>>5&7)
#define FAT_GET_SEC(T) (*(T)<<1)

FDIR *fat_opendir(DRIVE*, const char *path);
FDIR *fat_opensubdir(FDIR *parent, const char *name);
void fat_closedir(FDIR*);
int fat_readdir(FDIR*, struct fat_dirent*);
void fat_rewinddir(FDIR*);
int fat_telldir(FDIR*);
void fat_seekdir(FDIR*, int);

int fat_mkdir(DRIVE*, const char *path);
int fat_rmdir(DRIVE*, const char *path);

#ifndef FAT_IMPL
struct fat_cp {void *dummy;};
#endif
typedef struct fat_cp *fat_cp_t;
extern struct fat_cp fat_cp850[1];
extern struct fat_cp fat_cp852[1];
int fat_set_cp(DRIVE*, fat_cp_t);

/* image info (will change!) */

struct fat_dstat {
	unsigned long size, capacity, free, bad; /* in sectors */
	unsigned char cluster_sh, sector_sh;
	enum {fat_fat12, fat_fat16, fat_fat32} fat_type;
	unsigned long serial;
	unsigned char oemid[8];
	char label[12]; /* string; XXX currently from bs - wrong */
};

/* flags: 1 - need free sec, 2 - need bad sec */
int fat_dstat(DRIVE *dv, struct fat_dstat *st, int flags);

/* application should set this to current time */
extern time_t fat_time;

/* supplement */
unsigned char fat_u_to_cp(DRIVE *dv, unsigned u); /* only >= 0x80 */
unsigned fat_cp_to_u(DRIVE *dv, unsigned char c); /* only >= 0x80 */
unsigned char fat_cp_lower(DRIVE *dv, unsigned char c); /* only >= 0x80 */

static inline char *fat_put_utf8(char *d, char *e, unsigned u)
{
	char *fat__put_utf8_p(char *d, char *e, unsigned u);
	if(d==e) return 0;
	if(u<0x80) {
		*d++ = u;
		return d;
	}
	return fat__put_utf8_p(d, e-1, u);
}
