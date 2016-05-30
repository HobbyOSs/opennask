/*
   Filename:     mkdosfs.c
   Version:      0.3b (Yggdrasil)
   Author:       Dave Hudson
   Started:      24th August 1994
   Last Updated: 7th May 1998
   Updated by:   Roman Hodek <Roman.Hodek@informatik.uni-erlangen.de>
   Target O/S:   Linux (2.x)

   Description: Utility to allow an MS-DOS filesystem to be created
   under Linux.  A lot of the basic structure of this program has been
   borrowed from Remy Card's "mke2fs" code.

   As far as possible the aim here is to make the "mkdosfs" command
   look almost identical to the other Linux filesystem make utilties,
   eg bad blocks are still specified as blocks, not sectors, but when
   it comes down to it, DOS is tied to the idea of a sector (512 bytes
   as a rule), and not the block.  For example the boot block does not
   occupy a full cluster.

   Fixes/additions May 1998 by Roman Hodek
   <Roman.Hodek@informatik.uni-erlangen.de>:
   - Atari format support
   - New options -A, -S, -C
   - Support for filesystems > 2GB
   - FAT32 support

   Port to work under Windows NT/2K/XP Dec 2002 by
   Jens-Uwe Mager <jum@anubis.han.de>

   Copying:     Copyright 1993, 1994 David Hudson (dave@humbug.demon.co.uk)

   Portions copyright 1992, 1993 Remy Card (card@masi.ibp.fr)
   and 1991 Linus Torvalds (torvalds@klaava.helsinki.fi)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA. */

#ifdef __cplusplus
extern "C" {
#endif

#include "../version.h"

#ifdef _WIN32
#define _WIN32_WINNT	0x0400
#include <windows.h>
#include <winioctl.h>
#define __LITTLE_ENDIAN	1234
#define __BIG_ENDIAN	4321
#define __BYTE_ORDER	__LITTLE_ENDIAN
#define inline
#define __attribute__(x)
#define BLOCK_SIZE		512
#else
#include <linux/hdreg.h>
#include <linux/fs.h>
#include <linux/fd.h>
#include <endian.h>
#include <mntent.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <unistd.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <errno.h>

#ifndef _WIN32
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 0)
# define __KERNEL__
# include <asm/types.h>
# undef __KERNEL__
#endif
#endif

#if __BYTE_ORDER == __BIG_ENDIAN

#include <asm/byteorder.h>
#ifdef __le16_to_cpu
/* ++roman: 2.1 kernel headers define these function, they're probably more
 * efficient then coding the swaps machine-independently. */
#define CF_LE_W	__le16_to_cpu
#define CF_LE_L	__le32_to_cpu
#define CT_LE_W	__cpu_to_le16
#define CT_LE_L	__cpu_to_le32
#else
#define CF_LE_W(v) ((((v) & 0xff) << 8) | (((v) >> 8) & 0xff))
#define CF_LE_L(v) (((unsigned)(v)>>24) | (((unsigned)(v)>>8)&0xff00) | \
               (((unsigned)(v)<<8)&0xff0000) | ((unsigned)(v)<<24))
#define CT_LE_W(v) CF_LE_W(v)
#define CT_LE_L(v) CF_LE_L(v)
#endif /* defined(__le16_to_cpu) */

#else

#define CF_LE_W(v) (v)
#define CF_LE_L(v) (v)
#define CT_LE_W(v) (v)
#define CT_LE_L(v) (v)

#endif /* __BIG_ENDIAN */

#ifdef _WIN32

typedef unsigned char __u8;
typedef unsigned short __u16;
typedef unsigned int __u32;
typedef unsigned __int64 __u64;
typedef __int64 loff_t;
#define off_t loff_t
typedef __int64 ll_t;

// XXX: Fixme: what is used for 64 bit on VC?
#define strtoull strtoul

extern char *optarg;
extern int optind;
extern int opterr;
extern int optopt;
int getopt(int argc, char *const argv[], const char * optstring);

static int is_device = 0;

#define open	WIN32open
#define close	WIN32close
#define read	WIN32read
#define write	WIN32write
#define llseek	WIN32llseek

#define O_SHORT_LIVED   _O_SHORT_LIVED
#define O_ACCMODE       3
#define O_NONE          3
#define O_BACKUP        0x10000
#define O_SHARED        0x20000

static int WIN32open(const char *path, int oflag, ...)
{
	HANDLE fh;
	DWORD desiredAccess;
	DWORD shareMode;
	DWORD creationDisposition;
	DWORD flagsAttributes = FILE_ATTRIBUTE_NORMAL;
	SECURITY_ATTRIBUTES securityAttributes;
	va_list ap;
	int pmode;
	int trunc = FALSE;

	securityAttributes.nLength = sizeof(securityAttributes);
	securityAttributes.lpSecurityDescriptor = NULL;
	securityAttributes.bInheritHandle = oflag & O_NOINHERIT ? FALSE : TRUE;
	switch (oflag & O_ACCMODE) {
	case O_RDONLY:
		desiredAccess = GENERIC_READ;
		shareMode = FILE_SHARE_READ;
		break;
	case O_WRONLY:
		desiredAccess = GENERIC_WRITE;
		shareMode = 0;
		break;
	case O_RDWR:
		desiredAccess = GENERIC_READ|GENERIC_WRITE;
		shareMode = 0;
		break;
	case O_NONE:
		desiredAccess = 0;
		shareMode = FILE_SHARE_READ|FILE_SHARE_WRITE;
	}
	if (oflag & O_APPEND) {
		desiredAccess |= FILE_APPEND_DATA|SYNCHRONIZE;
		shareMode = FILE_SHARE_READ|FILE_SHARE_WRITE;
	}
	if (oflag & O_SHARED)
		shareMode |= FILE_SHARE_READ|FILE_SHARE_WRITE;
        switch (oflag & (O_CREAT|O_EXCL|O_TRUNC)) {
	case 0:
	case O_EXCL:
		creationDisposition = OPEN_EXISTING;
		break;
	case O_CREAT:
		creationDisposition = OPEN_ALWAYS;
		break;
	case O_CREAT|O_EXCL:
	case O_CREAT|O_TRUNC|O_EXCL:
		creationDisposition = CREATE_NEW;
		break;
	case O_TRUNC:
	case O_TRUNC|O_EXCL:
		creationDisposition = TRUNCATE_EXISTING;
		break;
	case O_CREAT|O_TRUNC:
		creationDisposition = OPEN_ALWAYS;
		trunc = TRUE;
		break;
        }
	if (oflag & O_CREAT) {
		va_start(ap, oflag);
		pmode = va_arg(ap, int);
		va_end(ap);
		if ((pmode & 0222) == 0)
			flagsAttributes |= FILE_ATTRIBUTE_READONLY;
	}
	if (oflag & O_TEMPORARY) {
		flagsAttributes |= FILE_FLAG_DELETE_ON_CLOSE;
		desiredAccess |= DELETE;
	}
	if (oflag & O_SHORT_LIVED)
		flagsAttributes |= FILE_ATTRIBUTE_TEMPORARY;
	if (oflag & O_SEQUENTIAL)
		flagsAttributes |= FILE_FLAG_SEQUENTIAL_SCAN;
	else if (oflag & O_RANDOM)
		flagsAttributes |= FILE_FLAG_RANDOM_ACCESS;
	if (oflag & O_BACKUP)
		flagsAttributes |= FILE_FLAG_BACKUP_SEMANTICS;
	if ((fh = CreateFile(path, desiredAccess, shareMode, &securityAttributes,
				creationDisposition, flagsAttributes, NULL)) == INVALID_HANDLE_VALUE) {
		errno = GetLastError();
		return -1;
	}
	if (trunc) {
		if (!SetEndOfFile(fh)) {
			errno = GetLastError();
			CloseHandle(fh);
			DeleteFile(path);
			return -1;
		}
	}
	return (int)fh;
}

static int WIN32close(int fd)
{
	if (!CloseHandle((HANDLE)fd)) {
		errno = GetLastError();
		return -1;
	}
	return 0;
}

static int WIN32read(int fd, void *buf, unsigned int len)
{
	DWORD actualLen;

	if (!ReadFile((HANDLE)fd, buf, (DWORD)len, &actualLen, NULL)) {
		errno = GetLastError();
		if (errno == ERROR_BROKEN_PIPE)
			return 0;
		else
			return -1;
	}
	return (int)actualLen;
}

static int WIN32write(int fd, void *buf, unsigned int len)
{
	DWORD actualLen;

	if (!WriteFile((HANDLE)fd, buf, (DWORD)len, &actualLen, NULL)) {
		errno = GetLastError();
		return -1;
	}
	return (int)actualLen;
}

static loff_t WIN32llseek(int fd, loff_t offset, int whence)
{
	long lo, hi;
	DWORD err;

	lo = offset & 0xffffffff;
	hi = offset >> 32;
	lo = SetFilePointer((HANDLE)fd, lo, &hi, whence);
	if (lo == 0xFFFFFFFF && (err = GetLastError()) != NO_ERROR) {
		errno = err;
		return -1;
	}
	return ((loff_t)hi << 32) | (off_t)lo;
}

int fsctl(int fd, int code)
{
	DWORD ret;
	if (!DeviceIoControl((HANDLE)fd, code, NULL, 0, NULL, 0, &ret, NULL)) {
		errno = GetLastError();
		return -1;
	}
	return 0;
}

void perror(const char *string)
{
	char *err = NULL;
	int e = errno;

        FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|
                FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK,
                NULL, e, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (char *)&err, 0, NULL );
        if (err == NULL) {
                FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_STRING|
                        FORMAT_MESSAGE_MAX_WIDTH_MASK|FORMAT_MESSAGE_ARGUMENT_ARRAY,
                        "Error %1!d!", 0, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                        (char *)&err, 0, (char **)&e);
        }
	fprintf(stderr, "%s: %s\n", string, err);
	LocalFree(err);
}

#else

#define O_NOINHERIT    0
#define O_TEMPORARY    0
#define O_SHORT_LIVED  0
#define O_SEQUENTIAL   0
#define O_RANDOM       0
#define O_BACKUP       0
#define O_SHARED       0
#ifndef O_NONE
# define O_NONE        0
#endif

typedef long long ll_t;

/* In earlier versions, an own llseek() was used, but glibc lseek() is
 * sufficient (or even better :) for 64 bit offsets in the meantime */
#define llseek lseek

#endif

/* Constant definitions */

#define TRUE 1			/* Boolean constants */
#define FALSE 0

#define TEST_BUFFER_BLOCKS 16
#define HARD_SECTOR_SIZE   512
#define SECTORS_PER_BLOCK ( BLOCK_SIZE / HARD_SECTOR_SIZE )


/* Macro definitions */

/* Report a failure message and return a failure error code */

#define die( str ) fatal_error( "%s: " str "\n" )


/* Mark a cluster in the FAT as bad */

#define mark_sector_bad( sector ) mark_FAT_sector( sector, FAT_BAD )

/* Compute ceil(a/b) */

inline int
cdiv (int a, int b)
{
  return (a + b - 1) / b;
}

/* MS-DOS filesystem structures -- I included them here instead of
   including linux/msdos_fs.h since that doesn't include some fields we
   need */

#define ATTR_RO      1		/* read-only */
#define ATTR_HIDDEN  2		/* hidden */
#define ATTR_SYS     4		/* system */
#define ATTR_VOLUME  8		/* volume label */
#define ATTR_DIR     16		/* directory */
#define ATTR_ARCH    32		/* archived */

#define ATTR_NONE    0		/* no attribute bits */
#define ATTR_UNUSED  (ATTR_VOLUME | ATTR_ARCH | ATTR_SYS | ATTR_HIDDEN)
	/* attribute bits that are copied "as is" */

/* FAT values */
#define FAT_EOF      (atari_format ? 0x0fffffff : 0x0ffffff8)
#define FAT_BAD      0x0ffffff7

#define MSDOS_EXT_SIGN 0x29	/* extended boot sector signature */
#define MSDOS_FAT12_SIGN "FAT12   "	/* FAT12 filesystem signature */
#define MSDOS_FAT16_SIGN "FAT16   "	/* FAT16 filesystem signature */
#define MSDOS_FAT32_SIGN "FAT32   "	/* FAT32 filesystem signature */

#define BOOT_SIGN 0xAA55	/* Boot sector magic number */

#define MAX_CLUST_12	((1 << 12) - 16)
#define MAX_CLUST_16	((1 << 16) - 16)
#define MIN_CLUST_32    65529
/* M$ says the high 4 bits of a FAT32 FAT entry are reserved and don't belong
 * to the cluster number. So the max. cluster# is based on 2^28 */
#define MAX_CLUST_32	((1 << 28) - 16)

#define FAT12_THRESHOLD	4085

#define OLDGEMDOS_MAX_SECTORS	32765
#define GEMDOS_MAX_SECTORS	65531
#define GEMDOS_MAX_SECTOR_SIZE	(16*1024)

#define BOOTCODE_SIZE		448
#define BOOTCODE_FAT32_SIZE	420

/* __attribute__ ((packed)) is used on all structures to make gcc ignore any
 * alignments */

#ifdef _WIN32
#pragma pack(push, 1)
#endif
struct msdos_volume_info {
  __u8		drive_number;	/* BIOS drive number */
  __u8		RESERVED;	/* Unused */
  __u8		ext_boot_sign;	/* 0x29 if fields below exist (DOS 3.3+) */
  __u8		volume_id[4];	/* Volume ID number */
  __u8		volume_label[11];/* Volume label */
  __u8		fs_type[8];	/* Typically FAT12 or FAT16 */
} __attribute__ ((packed));

struct msdos_boot_sector
{
  __u8	        boot_jump[3];	/* Boot strap short or near jump */
  __u8          system_id[8];	/* Name - can be used to special case
				   partition manager volumes */
  __u8          sector_size[2];	/* bytes per logical sector */
  __u8          cluster_size;	/* sectors/cluster */
  __u16         reserved;	/* reserved sectors */
  __u8          fats;		/* number of FATs */
  __u8          dir_entries[2];	/* root directory entries */
  __u8          sectors[2];	/* number of sectors */
  __u8          media;		/* media code (unused) */
  __u16         fat_length;	/* sectors/FAT */
  __u16         secs_track;	/* sectors per track */
  __u16         heads;		/* number of heads */
  __u32         hidden;		/* hidden sectors (unused) */
  __u32         total_sect;	/* number of sectors (if sectors == 0) */
  union {
    struct {
      struct msdos_volume_info vi;
      __u8	boot_code[BOOTCODE_SIZE];
    } __attribute__ ((packed)) _oldfat;
    struct {
      __u32	fat32_length;	/* sectors/FAT */
      __u16	flags;		/* bit 8: fat mirroring, low 4: active fat */
      __u8	version[2];	/* major, minor filesystem version */
      __u32	root_cluster;	/* first cluster in root directory */
      __u16	info_sector;	/* filesystem info sector */
      __u16	backup_boot;	/* backup boot sector */
      __u16	reserved2[6];	/* Unused */
      struct msdos_volume_info vi;
      __u8	boot_code[BOOTCODE_FAT32_SIZE];
    } __attribute__ ((packed)) _fat32;
  } __attribute__ ((packed)) fstype;
  __u16		boot_sign;
} __attribute__ ((packed));
#define fat32	fstype._fat32
#define oldfat	fstype._oldfat

struct fat32_fsinfo {
  __u32		reserved1;	/* Nothing as far as I can tell */
  __u32		signature;	/* 0x61417272L */
  __u32		free_clusters;	/* Free cluster count.  -1 if unknown */
  __u32		next_cluster;	/* Most recently allocated cluster.
				 * Unused under Linux. */
  __u32		reserved2[4];
};

struct msdos_dir_entry
  {
    char	name[8], ext[3];	/* name and extension */
    __u8        attr;			/* attribute bits */
    __u8	lcase;			/* Case for base and extension */
    __u8	ctime_ms;		/* Creation time, milliseconds */
    __u16	ctime;			/* Creation time */
    __u16	cdate;			/* Creation date */
    __u16	adate;			/* Last access date */
    __u16	starthi;		/* high 16 bits of first cl. (FAT32) */
    __u16	time, date, start;	/* time, date and first cluster */
    __u32	size;			/* file size (in bytes) */
  } __attribute__ ((packed));

#ifdef _WIN32
#pragma pack(pop)
#endif

/* The "boot code" we put into the filesystem... it writes a message and
   tells the user to try again */

static unsigned char dummy_boot_jump[3] = { 0xeb, 0x3c, 0x90 };

static char dummy_boot_jump_m68k[2] = { 0x60, 0x1c };

#define MSG_OFFSET_OFFSET 3
static char dummy_boot_code[BOOTCODE_SIZE] =
  "\x0e"			/* push cs */
  "\x1f"			/* pop ds */
  "\xbe\x5b\x7c"		/* mov si, offset message_txt */
				/* write_msg: */
  "\xac"			/* lodsb */
  "\x22\xc0"			/* and al, al */
  "\x74\x0b"			/* jz key_press */
  "\x56"			/* push si */
  "\xb4\x0e"			/* mov ah, 0eh */
  "\xbb\x07\x00"		/* mov bx, 0007h */
  "\xcd\x10"			/* int 10h */
  "\x5e"			/* pop si */
  "\xeb\xf0"			/* jmp write_msg */
				/* key_press: */
  "\x32\xe4"			/* xor ah, ah */
  "\xcd\x16"			/* int 16h */
  "\xcd\x19"			/* int 19h */
  "\xeb\xfe"			/* foo: jmp foo */
				/* message_txt: */

  "This is not a bootable disk.  Please insert a bootable floppy and\r\n"
  "press any key to try again ... \r\n";

#define MESSAGE_OFFSET 29	/* Offset of message in above code */

/* Global variables - the root of all evil :-) - see these and weep! */

static const char *program_name = "mkdosfs";	/* Name of the program */
static char *device_name = NULL;	/* Name of the device on which to create the filesystem */
static int atari_format = 0;	/* Use Atari variation of MS-DOS FS format */
static int check = FALSE;	/* Default to no readablity checking */
static int verbose = 0;		/* Default to verbose mode off */
static long volume_id;		/* Volume ID number */
static time_t create_time;	/* Creation time */
static char volume_name[] = "           "; /* Volume name */
static unsigned long long blocks;	/* Number of blocks in filesystem */
static int sector_size = 512;	/* Size of a logical sector */
static int sector_size_set = 0; /* User selected sector size */
static int backup_boot = 0;	/* Sector# of backup boot sector */
static int reserved_sectors = 0;/* Number of reserved sectors */
static int badblocks = 0;	/* Number of bad blocks in the filesystem */
static int nr_fats = 2;		/* Default number of FATs to produce */
static int size_fat = 0;	/* Size in bits of FAT entries */
static int size_fat_by_user = 0; /* 1 if FAT size user selected */
static int dev = -1;		/* FS block device file handle */
static int  ignore_full_disk = 0; /* Ignore warning about 'full' disk devices */
static off_t currently_testing = 0;	/* Block currently being tested (if autodetect bad blocks) */
static struct msdos_boot_sector bs;	/* Boot sector data */
static int start_data_sector;	/* Sector number for the start of the data area */
static int start_data_block;	/* Block number for the start of the data area */
static unsigned char *fat;	/* File allocation table */
static unsigned char *info_sector;	/* FAT32 info sector */
static struct msdos_dir_entry *root_dir;	/* Root directory */
static int size_root_dir;	/* Size of the root directory in bytes */
static int sectors_per_cluster = 0;	/* Number of sectors per disk cluster */
static int root_dir_entries = 0;	/* Number of root directory entries */
static char *blank_sector;		/* Blank sector - all zeros */
static int hidden_sectors = 0;		/* Number of hidden sectors */


/* Function prototype definitions */

 void fatal_error (const char *fmt_string) __attribute__((noreturn));
 void mark_FAT_cluster (int cluster, unsigned int value);
 void mark_FAT_sector (int sector, unsigned int value);
 long do_check (char *buffer, int t, off_t current_block);
 void alarm_intr (int alnum);
 void check_atari();
 void check_blocks (void);
 void get_list_blocks (char *filename);
 int valid_offset (int fd, loff_t offset);
 unsigned long long count_blocks (char *filename);
 void check_mount (char *device_name);
#ifdef _WIN32
 void establish_params (void);
#else
 void establish_params (int device_num, int size);
#endif
 void setup_tables (void);
 void write_tables (void);

#ifdef __cplusplus
};
#endif
