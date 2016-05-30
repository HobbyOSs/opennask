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


/* Include the header files */
#include "mkdosfs.h"

/* The function implementations */
#ifdef __cplusplus
extern "C" {
#endif

/* Handle the reporting of fatal errors.  Volatile to let gcc know that this doesn't return */

void
fatal_error (const char *fmt_string)
{
  fprintf (stderr, fmt_string, program_name, device_name);
  exit (1);			/* The error exit code is 1! */
}


/* Mark the specified cluster as having a particular value */

void
mark_FAT_cluster (int cluster, unsigned int value)
{
  switch( size_fat ) {
    case 12:
      value &= 0x0fff;
      if (((cluster * 3) & 0x1) == 0)
	{
	  fat[3 * cluster / 2] = (unsigned char) (value & 0x00ff);
	  fat[(3 * cluster / 2) + 1] = (unsigned char) ((fat[(3 * cluster / 2) + 1] & 0x00f0)
						 | ((value & 0x0f00) >> 8));
	}
      else
	{
	  fat[3 * cluster / 2] = (unsigned char) ((fat[3 * cluster / 2] & 0x000f) | ((value & 0x000f) << 4));
	  fat[(3 * cluster / 2) + 1] = (unsigned char) ((value & 0x0ff0) >> 4);
	}
      break;

    case 16:
      value &= 0xffff;
      fat[2 * cluster] = (unsigned char) (value & 0x00ff);
      fat[(2 * cluster) + 1] = (unsigned char) (value >> 8);
      break;

    case 32:
      value &= 0xfffffff;
      fat[4 * cluster] =       (unsigned char)  (value & 0x000000ff);
      fat[(4 * cluster) + 1] = (unsigned char) ((value & 0x0000ff00) >> 8);
      fat[(4 * cluster) + 2] = (unsigned char) ((value & 0x00ff0000) >> 16);
      fat[(4 * cluster) + 3] = (unsigned char) ((value & 0xff000000) >> 24);
      break;

    default:
      die("Bad FAT size (not 12, 16, or 32)");
  }
}


/* Mark a specified sector as having a particular value in it's FAT entry */

void
mark_FAT_sector (int sector, unsigned int value)
{
  int cluster;

  cluster = (sector - start_data_sector) / (int) (bs.cluster_size) /
	    (sector_size/HARD_SECTOR_SIZE);
  if (cluster < 0)
    die ("Invalid cluster number in mark_FAT_sector: probably bug!");

  mark_FAT_cluster (cluster, value);
}


/* Perform a test on a block.  Return the number of blocks that could be read successfully */

long
do_check (char *buffer, int try, off_t current_block)
{
  long got;

  if (llseek (dev, current_block * BLOCK_SIZE, SEEK_SET) /* Seek to the correct location */
      != current_block * BLOCK_SIZE)
    die ("seek failed during testing for blocks");

  got = read (dev, buffer, try * BLOCK_SIZE);	/* Try reading! */
  if (got < 0)
    got = 0;

  if (got & (BLOCK_SIZE - 1))
    printf ("Unexpected values in do_check: probably bugs\n");
  got /= BLOCK_SIZE;

  return got;
}

#ifndef _WIN32
/* Alarm clock handler - display the status of the quest for bad blocks!  Then retrigger the alarm for five senconds
   later (so we can come here again) */

void
alarm_intr (int alnum)
{
  if (currently_testing >= blocks)
    return;

  signal (SIGALRM, alarm_intr);
  alarm (5);
  if (!currently_testing)
    return;

  printf ("%lld... ", (unsigned long long)currently_testing);
  fflush (stdout);
}
#endif

void
check_blocks (void)
{
  int try, got;
  int i;
  static char blkbuf[BLOCK_SIZE * TEST_BUFFER_BLOCKS];

  if (verbose)
    {
      printf ("Searching for bad blocks ");
      fflush (stdout);
    }
  currently_testing = 0;
#ifndef _WIN32
  if (verbose)
    {
      signal (SIGALRM, alarm_intr);
      alarm (5);
    }
#endif
  try = TEST_BUFFER_BLOCKS;
  while (currently_testing < blocks)
    {
      if (currently_testing + try > blocks)
	try = blocks - currently_testing;
      got = do_check (blkbuf, try, currently_testing);
      currently_testing += got;
      if (got == try)
	{
	  try = TEST_BUFFER_BLOCKS;
	  continue;
	}
      else
	try = 1;
      if (currently_testing < start_data_block)
	die ("bad blocks before data-area: cannot make fs");

      for (i = 0; i < SECTORS_PER_BLOCK; i++)	/* Mark all of the sectors in the block as bad */
	mark_sector_bad (currently_testing * SECTORS_PER_BLOCK + i);
      badblocks++;
      currently_testing++;
    }

  if (verbose)
    printf ("\n");

  if (badblocks)
    printf ("%d bad block%s\n", badblocks,
	    (badblocks > 1) ? "s" : "");
}


void
get_list_blocks (char *filename)
{
  int i;
  FILE *listfile;
  unsigned long blockno;

  listfile = fopen (filename, "r");
  if (listfile == (FILE *) NULL)
    die ("Can't open file of bad blocks");

  while (!feof (listfile))
    {
      fscanf (listfile, "%ld\n", &blockno);
      for (i = 0; i < SECTORS_PER_BLOCK; i++)	/* Mark all of the sectors in the block as bad */
	mark_sector_bad (blockno * SECTORS_PER_BLOCK + i);
      badblocks++;
    }
  fclose (listfile);

  if (badblocks)
    printf ("%d bad block%s\n", badblocks,
	    (badblocks > 1) ? "s" : "");
}


#ifndef _WIN32
/* Given a file descriptor and an offset, check whether the offset is a valid offset for the file - return FALSE if it
   isn't valid or TRUE if it is */

int
valid_offset (int fd, loff_t offset)
{
  char ch;

  if (llseek (fd, offset, SEEK_SET) < 0)
    return FALSE;
  if (read (fd, &ch, 1) < 1)
    return FALSE;
  return TRUE;
}
#endif


/* Given a filename, look to see how many blocks of BLOCK_SIZE are present, returning the answer */

unsigned long long
count_blocks (char *filename)
{
#ifdef _WIN32
	int fd;
	DISK_GEOMETRY geom;
	BY_HANDLE_FILE_INFORMATION hinfo;
	PARTITION_INFORMATION part_info;
	DWORD ret;
	loff_t len = 0;

	if ((fd = open(filename, O_RDONLY)) < 0) {
		perror(filename);
		exit(1);
	}
	/*
	 * This should probably use IOCTL_DISK_GET_LENGTH_INFO here, but
	 * this ioctl is only available in XP and up.
	 *
	 * PATCHED: the IOCTL_DISK_GET_DRIVE_GEOMETRY does not return hidden sectors
	 * in the geometry which causes problems for larger partitions
	 * therefore we call IOCTL_DISK_GET_PARTITION_INFO to get part_info.PartitionLength
	 * and then divide by geom.BytesPerSector
	 */
	if (is_device) {
		if (!DeviceIoControl((HANDLE)fd, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &geom, sizeof(geom), &ret, NULL) ||
			!DeviceIoControl((HANDLE)fd, IOCTL_DISK_GET_PARTITION_INFO, NULL, 0, &part_info, sizeof(part_info), &ret, NULL)) {
			errno = GetLastError();
			die("unable to get length for '%s'");
		}

		//len = (loff_t)geom.Cylinders.QuadPart*(loff_t)geom.TracksPerCylinder*(loff_t)geom.SectorsPerTrack*(loff_t)BLOCK_SIZE;
		len = ((loff_t)part_info.PartitionLength.QuadPart / (loff_t)geom.BytesPerSector)*(loff_t)BLOCK_SIZE;

	} else {
		if (!GetFileInformationByHandle((HANDLE)fd, &hinfo)) {
				errno = GetLastError();
				die("unable to get length for '%s'");
		}
		len = ((loff_t)hinfo.nFileSizeHigh << 32) | (loff_t)hinfo.nFileSizeLow;
	}
	close(fd);
	return len/BLOCK_SIZE;
#else
  off_t high, low;
  int fd;

  if ((fd = open (filename, O_RDONLY)) < 0)
    {
      perror (filename);
      exit (1);
    }

  /* first try SEEK_END, which should work on most devices nowadays */
  if ((low = llseek(fd, 0, SEEK_END)) <= 0) {
      low = 0;
      for (high = 1; valid_offset (fd, high); high *= 2)
	  low = high;
      while (low < high - 1) {
	  const loff_t mid = (low + high) / 2;
	  if (valid_offset (fd, mid))
	      low = mid;
	  else
	      high = mid;
      }
      ++low;
  }

  close (fd);
  return low / BLOCK_SIZE;
#endif
}


/* Check to see if the specified device is currently mounted - abort if it is */

void
check_mount (char *device_name)
{
#ifndef _WIN32
  FILE *f;
  struct mntent *mnt;

  if ((f = setmntent (MOUNTED, "r")) == NULL)
    return;
  while ((mnt = getmntent (f)) != NULL)
    if (strcmp (device_name, mnt->mnt_fsname) == 0)
      die ("%s contains a mounted file system.");
  endmntent (f);
#endif
}


/* Establish the geometry and media parameters for the device */
#ifdef _WIN32
static void
establish_params (void)
{
	DISK_GEOMETRY geometry;
	DWORD ret;

	if (!is_device) {
		bs.media = (char) 0xf8; /* Set up the media descriptor for a hard drive */
		bs.dir_entries[0] = (char) 0;
		bs.dir_entries[1] = (char) 2;
		/* For FAT32, use 4k clusters on sufficiently large file systems,
		 * otherwise 1 sector per cluster. This is also what M$'s format
		 * command does for FAT32. */
		bs.cluster_size = (char)
		 (size_fat == 32 ?
	     ((ll_t)blocks*SECTORS_PER_BLOCK >= 512*1024 ? 8 : 1) :
	      4); /* FAT12 and FAT16: start at 4 sectors per cluster */
		return;
	}
	if (!DeviceIoControl((HANDLE)dev, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &geometry, sizeof(geometry), &ret, NULL)) {
		errno = GetLastError();
		die ("unable to get geometry for '%s'");
	}
    bs.secs_track = geometry.SectorsPerTrack;
    bs.heads = geometry.TracksPerCylinder;
	switch (geometry.MediaType) {
	case F3_1Pt44_512:
		bs.media = (char) 0xf9;
		bs.cluster_size = (char) 2;
		bs.dir_entries[0] = (char) 112;
		bs.dir_entries[1] = (char) 0;
		break;
	case F3_2Pt88_512:
		bs.media = (char) 0xf0;
		bs.cluster_size = (char)(atari_format ? 2 : 1);
		bs.dir_entries[0] = (char) 224;
		bs.dir_entries[1] = (char) 0;
		break;
	case F3_720_512:
		bs.media = (char) 0xfd;
		bs.cluster_size = (char) 2;
		bs.dir_entries[0] = (char) 112;
		bs.dir_entries[1] = (char) 0;
		break;
	default:
		bs.media = (char) 0xf8; /* Set up the media descriptor for a hard drive */
		bs.dir_entries[0] = (char) 0;
		bs.dir_entries[1] = (char) 2;
		/* For FAT32, use 4k clusters on sufficiently large file systems,
		 * otherwise 1 sector per cluster. This is also what M$'s format
		 * command does for FAT32. */
		bs.cluster_size = (char)
		 (size_fat == 32 ?
	     ((ll_t)blocks*SECTORS_PER_BLOCK >= 512*1024 ? 8 : 1) :
	      4); /* FAT12 and FAT16: start at 4 sectors per cluster */
	}
}
#else
void
establish_params (int device_num,int size)
{
  long loop_size;
  struct hd_geometry geometry;
  struct floppy_struct param;

  if ((0 == device_num) || ((device_num & 0xff00) == 0x0200))
    /* file image or floppy disk */
    {
      if (0 == device_num)
	{
	  param.size = size/512;
	  switch(param.size)
	    {
	    case 720:
	      param.sect = 9 ;
	      param.head = 2;
	      break;
	    case 1440:
	      param.sect = 9;
	      param.head = 2;
	      break;
	    case 2400:
	      param.sect = 15;
	      param.head = 2;
	      break;
	    case 2880:
	      param.sect = 18;
	      param.head = 2;
	      break;
	    case 5760:
	      param.sect = 36;
	      param.head = 2;
	      break;
	    default:
	      /* fake values */
	      param.sect = 32;
	      param.head = 64;
	      break;
	    }

	}
      else 	/* is a floppy diskette */
	{
	  if (ioctl (dev, FDGETPRM, &param))	/*  Can we get the diskette geometry? */
	    die ("unable to get diskette geometry for '%s'");
	}
      bs.secs_track = CT_LE_W(param.sect);	/*  Set up the geometry information */
      bs.heads = CT_LE_W(param.head);
      switch (param.size)	/*  Set up the media descriptor byte */
	{
	case 720:		/* 5.25", 2, 9, 40 - 360K */
	  bs.media = (char) 0xfd;
	  bs.cluster_size = (char) 2;
	  bs.dir_entries[0] = (char) 112;
	  bs.dir_entries[1] = (char) 0;
	  break;

	case 1440:		/* 3.5", 2, 9, 80 - 720K */
	  bs.media = (char) 0xf9;
	  bs.cluster_size = (char) 2;
	  bs.dir_entries[0] = (char) 112;
	  bs.dir_entries[1] = (char) 0;
	  break;

	case 2400:		/* 5.25", 2, 15, 80 - 1200K */
	  bs.media = (char) 0xf9;
	  bs.cluster_size = (char)(atari_format ? 2 : 1);
	  bs.dir_entries[0] = (char) 224;
	  bs.dir_entries[1] = (char) 0;
	  break;

	case 5760:		/* 3.5", 2, 36, 80 - 2880K */
	  bs.media = (char) 0xf0;
	  bs.cluster_size = (char) 2;
	  bs.dir_entries[0] = (char) 224;
	  bs.dir_entries[1] = (char) 0;
	  break;

	case 2880:		/* 3.5", 2, 18, 80 - 1440K */
	floppy_default:
	  bs.media = (char) 0xf0;
	  bs.cluster_size = (char)(atari_format ? 2 : 1);
	  bs.dir_entries[0] = (char) 224;
	  bs.dir_entries[1] = (char) 0;
	  break;

	default:		/* Anything else */
	  if (0 == device_num)
	      goto def_hd_params;
	  else
	      goto floppy_default;
	}
    }
  else if ((device_num & 0xff00) == 0x0700) /* This is a loop device */
    {
      if (ioctl (dev, BLKGETSIZE, &loop_size))
	die ("unable to get loop device size");

      switch (loop_size)  /* Assuming the loop device -> floppy later */
	{
	case 720:		/* 5.25", 2, 9, 40 - 360K */
	  bs.secs_track = CF_LE_W(9);
	  bs.heads = CF_LE_W(2);
	  bs.media = (char) 0xfd;
	  bs.cluster_size = (char) 2;
	  bs.dir_entries[0] = (char) 112;
	  bs.dir_entries[1] = (char) 0;
	  break;

	case 1440:		/* 3.5", 2, 9, 80 - 720K */
	  bs.secs_track = CF_LE_W(9);
	  bs.heads = CF_LE_W(2);
	  bs.media = (char) 0xf9;
	  bs.cluster_size = (char) 2;
	  bs.dir_entries[0] = (char) 112;
	  bs.dir_entries[1] = (char) 0;
	  break;

	case 2400:		/* 5.25", 2, 15, 80 - 1200K */
	  bs.secs_track = CF_LE_W(15);
	  bs.heads = CF_LE_W(2);
	  bs.media = (char) 0xf9;
	  bs.cluster_size = (char)(atari_format ? 2 : 1);
	  bs.dir_entries[0] = (char) 224;
	  bs.dir_entries[1] = (char) 0;
	  break;

	case 5760:		/* 3.5", 2, 36, 80 - 2880K */
	  bs.secs_track = CF_LE_W(36);
	  bs.heads = CF_LE_W(2);
	  bs.media = (char) 0xf0;
	  bs.cluster_size = (char) 2;
	  bs.dir_entries[0] = (char) 224;
	  bs.dir_entries[1] = (char) 0;
	  break;

	case 2880:		/* 3.5", 2, 18, 80 - 1440K */
	  bs.secs_track = CF_LE_W(18);
	  bs.heads = CF_LE_W(2);
	  bs.media = (char) 0xf0;
	  bs.cluster_size = (char)(atari_format ? 2 : 1);
	  bs.dir_entries[0] = (char) 224;
	  bs.dir_entries[1] = (char) 0;
	  break;

	default:		/* Anything else: default hd setup */
	  printf("Loop device does not match a floppy size, using "
		 "default hd params\n");
	  bs.secs_track = CT_LE_W(32); /* these are fake values... */
	  bs.heads = CT_LE_W(64);
	  goto def_hd_params;
	}
    }
  else
    /* Must be a hard disk then! */
    {
      /* Can we get the drive geometry? (Note I'm not too sure about */
      /* whether to use HDIO_GETGEO or HDIO_REQ) */
      if (ioctl (dev, HDIO_GETGEO, &geometry)) {
	printf ("unable to get drive geometry, using default 255/63");
        bs.secs_track = CT_LE_W(63);
        bs.heads = CT_LE_W(255);
      }
      else {
        bs.secs_track = CT_LE_W(geometry.sectors);	/* Set up the geometry information */
        bs.heads = CT_LE_W(geometry.heads);
      }
    def_hd_params:
      bs.media = (char) 0xf8; /* Set up the media descriptor for a hard drive */
      bs.dir_entries[0] = (char) 0;	/* Default to 512 entries */
      bs.dir_entries[1] = (char) 2;
      if (!size_fat && blocks*SECTORS_PER_BLOCK > 1064960) {
	  if (verbose) printf("Auto-selecting FAT32 for large filesystem\n");
	  size_fat = 32;
      }
      if (size_fat == 32) {
	  /* For FAT32, try to do the same as M$'s format command:
	   * fs size < 256M: 0.5k clusters
	   * fs size <   8G: 4k clusters
	   * fs size <  16G: 8k clusters
	   * fs size >= 16G: 16k clusters
	   */
	  unsigned long sz_mb =
	      (blocks+(1<<(20-BLOCK_SIZE_BITS))-1) >> (20-BLOCK_SIZE_BITS);
	  bs.cluster_size = sz_mb >= 16*1024 ? 32 :
			    sz_mb >=  8*1024 ? 16 :
			    sz_mb >=     256 ?  8 :
					        1;
      }
      else {
	  /* FAT12 and FAT16: start at 4 sectors per cluster */
	  bs.cluster_size = (char) 4;
      }
    }
}
#endif


/* Create the filesystem data tables */

void
setup_tables (void)
{
  unsigned num_sectors;
  unsigned cluster_count = 0, fat_length;
  unsigned fatdata;			/* Sectors for FATs + data area */
  struct tm *ctime;
  struct msdos_volume_info *vi = (size_fat == 32 ? &bs.fat32.vi : &bs.oldfat.vi);

  if (atari_format)
      /* On Atari, the first few bytes of the boot sector are assigned
       * differently: The jump code is only 2 bytes (and m68k machine code
       * :-), then 6 bytes filler (ignored), then 3 byte serial number. */
    strncpy( bs.system_id-1, "mkdosf", 6 );
  else
    strcpy (bs.system_id, "mkdosfs");
  if (sectors_per_cluster)
    bs.cluster_size = (char) sectors_per_cluster;
  if (size_fat == 32)
    {
      /* Under FAT32, the root dir is in a cluster chain, and this is
       * signalled by bs.dir_entries being 0. */
      bs.dir_entries[0] = bs.dir_entries[1] = (char) 0;
      root_dir_entries = 0;
    }
  else if (root_dir_entries)
    {
      /* Override default from establish_params() */
      bs.dir_entries[0] = (char) (root_dir_entries & 0x00ff);
      bs.dir_entries[1] = (char) ((root_dir_entries & 0xff00) >> 8);
    }
  else
    root_dir_entries = bs.dir_entries[0] + (bs.dir_entries[1] << 8);

  if (atari_format) {
    bs.system_id[5] = (unsigned char) (volume_id & 0x000000ff);
    bs.system_id[6] = (unsigned char) ((volume_id & 0x0000ff00) >> 8);
    bs.system_id[7] = (unsigned char) ((volume_id & 0x00ff0000) >> 16);
  }
  else {
    vi->volume_id[0] = (unsigned char) (volume_id & 0x000000ff);
    vi->volume_id[1] = (unsigned char) ((volume_id & 0x0000ff00) >> 8);
    vi->volume_id[2] = (unsigned char) ((volume_id & 0x00ff0000) >> 16);
    vi->volume_id[3] = (unsigned char) (volume_id >> 24);
  }

  if (!atari_format) {
    memcpy(vi->volume_label, volume_name, 11);

    memcpy(bs.boot_jump, dummy_boot_jump, 3);
    /* Patch in the correct offset to the boot code */
    bs.boot_jump[1] = ((size_fat == 32 ?
			(char *)&bs.fat32.boot_code :
			(char *)&bs.oldfat.boot_code) -
		       (char *)&bs) - 2;

    if (size_fat == 32) {
	int offset = (char *)&bs.fat32.boot_code -
		     (char *)&bs + MESSAGE_OFFSET + 0x7c00;
	if (dummy_boot_code[BOOTCODE_FAT32_SIZE-1])
	  printf ("Warning: message too long; truncated\n");
	dummy_boot_code[BOOTCODE_FAT32_SIZE-1] = 0;
	memcpy(bs.fat32.boot_code, dummy_boot_code, BOOTCODE_FAT32_SIZE);
	bs.fat32.boot_code[MSG_OFFSET_OFFSET] = offset & 0xff;
	bs.fat32.boot_code[MSG_OFFSET_OFFSET+1] = offset >> 8;
    }
    else {
	memcpy(bs.oldfat.boot_code, dummy_boot_code, BOOTCODE_SIZE);
    }
    bs.boot_sign = CT_LE_W(BOOT_SIGN);
  }
  else {
    memcpy(bs.boot_jump, dummy_boot_jump_m68k, 2);
  }
  if (verbose >= 2)
    printf( "Boot jump code is %02x %02x\n",
	    bs.boot_jump[0], bs.boot_jump[1] );

  if (!reserved_sectors)
      reserved_sectors = (size_fat == 32) ? 32 : 1;
  else {
      if (size_fat == 32 && reserved_sectors < 2)
	  die("On FAT32 at least 2 reserved sectors are needed.");
  }
  bs.reserved = CT_LE_W(reserved_sectors);
  if (verbose >= 2)
    printf( "Using %d reserved sectors\n", reserved_sectors );
  bs.fats = (char) nr_fats;
  if (!atari_format || size_fat == 32)
    bs.hidden = CT_LE_L(hidden_sectors);
  else {
    /* In Atari format, hidden is a 16 bit field */
    __u16 hidden = CT_LE_W(hidden_sectors);
    if (hidden_sectors & ~0xffff)
      die("#hidden doesn't fit in 16bit field of Atari format\n");
    memcpy( &bs.hidden, &hidden, 2 );
  }

  num_sectors = (ll_t)blocks*BLOCK_SIZE/sector_size;
  if (!atari_format) {
    unsigned fatlength12, fatlength16, fatlength32;
    unsigned maxclust12, maxclust16, maxclust32;
    unsigned clust12, clust16, clust32;
    int maxclustsize;

    fatdata = num_sectors - cdiv (root_dir_entries * 32, sector_size) -
	      reserved_sectors;

    if (sectors_per_cluster)
      bs.cluster_size = maxclustsize = sectors_per_cluster;
    else
      /* An initial guess for bs.cluster_size should already be set */
      maxclustsize = 128;

    if (verbose >= 2)
      printf( "%d sectors for FAT+data, starting with %d sectors/cluster\n",
	      fatdata, bs.cluster_size );
    do {
      if (verbose >= 2)
	printf( "Trying with %d sectors/cluster:\n", bs.cluster_size );

      /* The factor 2 below avoids cut-off errors for nr_fats == 1.
       * The "nr_fats*3" is for the reserved first two FAT entries */
      clust12 = 2*((ll_t) fatdata *sector_size + nr_fats*3) /
	(2*(int) bs.cluster_size * sector_size + nr_fats*3);
      fatlength12 = cdiv (((clust12+2) * 3 + 1) >> 1, sector_size);
      /* Need to recalculate number of clusters, since the unused parts of the
       * FATS and data area together could make up space for an additional,
       * not really present cluster. */
      clust12 = (fatdata - nr_fats*fatlength12)/bs.cluster_size;
      maxclust12 = (fatlength12 * 2 * sector_size) / 3;
      if (maxclust12 > MAX_CLUST_12)
	maxclust12 = MAX_CLUST_12;
      if (verbose >= 2)
	printf( "FAT12: #clu=%u, fatlen=%u, maxclu=%u, limit=%u\n",
		clust12, fatlength12, maxclust12, MAX_CLUST_12 );
      if (clust12 > maxclust12-2) {
	clust12 = 0;
	if (verbose >= 2)
	  printf( "FAT12: too much clusters\n" );
      }

      clust16 = ((ll_t) fatdata *sector_size + nr_fats*4) /
	((int) bs.cluster_size * sector_size + nr_fats*2);
      fatlength16 = cdiv ((clust16+2) * 2, sector_size);
      /* Need to recalculate number of clusters, since the unused parts of the
       * FATS and data area together could make up space for an additional,
       * not really present cluster. */
      clust16 = (fatdata - nr_fats*fatlength16)/bs.cluster_size;
      maxclust16 = (fatlength16 * sector_size) / 2;
      if (maxclust16 > MAX_CLUST_16)
	maxclust16 = MAX_CLUST_16;
      if (verbose >= 2)
	printf( "FAT16: #clu=%u, fatlen=%u, maxclu=%u, limit=%u\n",
		clust16, fatlength16, maxclust16, MAX_CLUST_16 );
      if (clust16 > maxclust16-2) {
	if (verbose >= 2)
	  printf( "FAT16: too much clusters\n" );
	clust16 = 0;
      }
      /* The < 4078 avoids that the filesystem will be misdetected as having a
       * 12 bit FAT. */
      if (clust16 < FAT12_THRESHOLD && !(size_fat_by_user && size_fat == 16)) {
	if (verbose >= 2)
	  printf( clust16 < FAT12_THRESHOLD ?
		  "FAT16: would be misdetected as FAT12\n" :
		  "FAT16: too much clusters\n" );
	clust16 = 0;
      }

      clust32 = ((ll_t) fatdata *sector_size + nr_fats*8) /
	((int) bs.cluster_size * sector_size + nr_fats*4);
      fatlength32 = cdiv ((clust32+2) * 4, sector_size);
      /* Need to recalculate number of clusters, since the unused parts of the
       * FATS and data area together could make up space for an additional,
       * not really present cluster. */
      clust32 = (fatdata - nr_fats*fatlength32)/bs.cluster_size;
      maxclust32 = (fatlength32 * sector_size) / 4;
      if (maxclust32 > MAX_CLUST_32)
	maxclust32 = MAX_CLUST_32;
      if (clust32 && clust32 < MIN_CLUST_32 && !(size_fat_by_user && size_fat == 32)) {
       clust32 = 0;
       if (verbose >= 2)
         printf( "FAT32: not enough clusters (%d)\n", MIN_CLUST_32);
      }
      if (verbose >= 2)
	printf( "FAT32: #clu=%u, fatlen=%u, maxclu=%u, limit=%u\n",
		clust32, fatlength32, maxclust32, MAX_CLUST_32 );
      if (clust32 > maxclust32) {
	clust32 = 0;
	if (verbose >= 2)
	  printf( "FAT32: too much clusters\n" );
      }

      if ((clust12 && (size_fat == 0 || size_fat == 12)) ||
	  (clust16 && (size_fat == 0 || size_fat == 16)) ||
	  (clust32 && size_fat == 32))
	break;

      bs.cluster_size <<= 1;
    } while (bs.cluster_size && bs.cluster_size <= maxclustsize);

    /* Use the optimal FAT size if not specified;
     * FAT32 is (not yet) choosen automatically */
    if (!size_fat) {
	size_fat = (clust16 > clust12) ? 16 : 12;
	if (verbose >= 2)
	  printf( "Choosing %d bits for FAT\n", size_fat );
    }

    switch (size_fat) {
      case 12:
	cluster_count = clust12;
	fat_length = fatlength12;
	bs.fat_length = CT_LE_W(fatlength12);
	memcpy(vi->fs_type, MSDOS_FAT12_SIGN, 8);
	break;

      case 16:
	if (clust16 < FAT12_THRESHOLD) {
	    if (size_fat_by_user) {
		fprintf( stderr, "WARNING: Not enough clusters for a "
			 "16 bit FAT! The filesystem will be\n"
			 "misinterpreted as having a 12 bit FAT without "
			 "mount option \"fat=16\".\n" );
	    }
	    else {
		fprintf( stderr, "This filesystem has an unfortunate size. "
			 "A 12 bit FAT cannot provide\n"
			 "enough clusters, but a 16 bit FAT takes up a little "
			 "bit more space so that\n"
			 "the total number of clusters becomes less than the "
			 "threshold value for\n"
			 "distinction between 12 and 16 bit FATs.\n" );
		die( "Make the file system a bit smaller manually." );
	    }
	}
	cluster_count = clust16;
	fat_length = fatlength16;
	bs.fat_length = CT_LE_W(fatlength16);
	memcpy(vi->fs_type, MSDOS_FAT16_SIGN, 8);
	break;

      case 32:
	cluster_count = clust32;
	fat_length = fatlength32;
	bs.fat_length = CT_LE_W(0);
	bs.fat32.fat32_length = CT_LE_L(fatlength32);
	memcpy(vi->fs_type, MSDOS_FAT32_SIGN, 8);
	break;

      default:
	die("FAT not 12, 16 or 32 bits");
    }
  }
  else {
    unsigned clusters, maxclust;

    /* GEMDOS always uses a 12 bit FAT on floppies, and always a 16 bit FAT on
     * hard disks. So use 12 bit if the size of the file system suggests that
     * this fs is for a floppy disk, if the user hasn't explicitly requested a
     * size.
     */
    if (!size_fat)
      size_fat = (num_sectors == 1440 || num_sectors == 2400 ||
		  num_sectors == 2880 || num_sectors == 5760) ? 12 : 16;
    if (verbose >= 2)
      printf( "Choosing %d bits for FAT\n", size_fat );

    /* Atari format: cluster size should be 2, except explicitly requested by
     * the user, since GEMDOS doesn't like other cluster sizes very much.
     * Instead, tune the sector size for the FS to fit.
     */
    bs.cluster_size = sectors_per_cluster ? sectors_per_cluster : 2;
    if (!sector_size_set) {
      while( num_sectors > GEMDOS_MAX_SECTORS ) {
	num_sectors >>= 1;
	sector_size <<= 1;
      }
    }
    if (verbose >= 2)
      printf( "Sector size must be %d to have less than %d log. sectors\n",
	      sector_size, GEMDOS_MAX_SECTORS );

    /* Check if there are enough FAT indices for how much clusters we have */
    do {
      fatdata = num_sectors - cdiv (root_dir_entries * 32, sector_size) -
		reserved_sectors;
      /* The factor 2 below avoids cut-off errors for nr_fats == 1 and
       * size_fat == 12
       * The "2*nr_fats*size_fat/8" is for the reserved first two FAT entries
       */
      clusters = (2*((ll_t)fatdata*sector_size - 2*nr_fats*size_fat/8)) /
		 (2*((int)bs.cluster_size*sector_size + nr_fats*size_fat/8));
      fat_length = cdiv( (clusters+2)*size_fat/8, sector_size );
      /* Need to recalculate number of clusters, since the unused parts of the
       * FATS and data area together could make up space for an additional,
       * not really present cluster. */
      clusters = (fatdata - nr_fats*fat_length)/bs.cluster_size;
      maxclust = (fat_length*sector_size*8)/size_fat;
      if (verbose >= 2)
	printf( "ss=%d: #clu=%d, fat_len=%d, maxclu=%d\n",
		sector_size, clusters, fat_length, maxclust );

      /* last 10 cluster numbers are special (except FAT32: 4 high bits rsvd);
       * first two numbers are reserved */
      if (maxclust <= (size_fat == 32 ? MAX_CLUST_32 : (1<<size_fat)-0x10) &&
	  clusters <= maxclust-2)
	break;
      if (verbose >= 2)
	printf( clusters > maxclust-2 ?
		"Too many clusters\n" : "FAT too big\n" );

      /* need to increment sector_size once more to  */
      if (sector_size_set)
	  die( "With this sector size, the maximum number of FAT entries "
	       "would be exceeded." );
      num_sectors >>= 1;
      sector_size <<= 1;
    } while( sector_size <= GEMDOS_MAX_SECTOR_SIZE );

    if (sector_size > GEMDOS_MAX_SECTOR_SIZE)
      die( "Would need a sector size > 16k, which GEMDOS can't work with");

    cluster_count = clusters;
    if (size_fat != 32)
	bs.fat_length = CT_LE_W(fat_length);
    else {
	bs.fat_length = 0;
	bs.fat32.fat32_length = CT_LE_L(fat_length);
    }
  }

  bs.sector_size[0] = (char) (sector_size & 0x00ff);
  bs.sector_size[1] = (char) ((sector_size & 0xff00) >> 8);

  if (size_fat == 32)
    {
      /* set up additional FAT32 fields */
      bs.fat32.flags = CT_LE_W(0);
      bs.fat32.version[0] = 0;
      bs.fat32.version[1] = 0;
      bs.fat32.root_cluster = CT_LE_L(2);
      bs.fat32.info_sector = CT_LE_W(1);
      if (!backup_boot)
	backup_boot = (reserved_sectors >= 7) ? 6 :
		      (reserved_sectors >= 2) ? reserved_sectors-1 : 0;
      else
	{
	  if (backup_boot == 1)
	    die("Backup boot sector must be after sector 1");
	  else if (backup_boot >= reserved_sectors)
	    die("Backup boot sector must be a reserved sector");
	}
      if (verbose >= 2)
	printf( "Using sector %d as backup boot sector (0 = none)\n",
		backup_boot );
      bs.fat32.backup_boot = CT_LE_W(backup_boot);
      memset( &bs.fat32.reserved2, 0, sizeof(bs.fat32.reserved2) );
    }

  if (atari_format) {
      /* Just some consistency checks */
      if (num_sectors >= GEMDOS_MAX_SECTORS)
	  die( "GEMDOS can't handle more than 65531 sectors" );
      else if (num_sectors >= OLDGEMDOS_MAX_SECTORS)
	  printf( "Warning: More than 32765 sector need TOS 1.04 "
		  "or higher.\n" );
  }
  if (num_sectors >= 65536)
    {
      bs.sectors[0] = (char) 0;
      bs.sectors[1] = (char) 0;
      bs.total_sect = CT_LE_L(num_sectors);
    }
  else
    {
      bs.sectors[0] = (char) (num_sectors & 0x00ff);
      bs.sectors[1] = (char) ((num_sectors & 0xff00) >> 8);
      if (!atari_format)
	  bs.total_sect = CT_LE_L(0);
    }

  if (!atari_format)
    vi->ext_boot_sign = MSDOS_EXT_SIGN;

  if (!cluster_count)
    {
      if (sectors_per_cluster)	/* If yes, die if we'd spec'd sectors per cluster */
	die ("Too many clusters for file system - try more sectors per cluster");
      else
	die ("Attempting to create a too large file system");
    }


  /* The two following vars are in hard sectors, i.e. 512 byte sectors! */
  start_data_sector = (reserved_sectors + nr_fats * fat_length) *
		      (sector_size/HARD_SECTOR_SIZE);
  start_data_block = (start_data_sector + SECTORS_PER_BLOCK - 1) /
		     SECTORS_PER_BLOCK;

  if (blocks < start_data_block + 32)	/* Arbitrary undersize file system! */
    die ("Too few blocks for viable file system");

  if (verbose)
    {
      printf("%s has %d head%s and %d sector%s per track,\n",
	     device_name, CF_LE_W(bs.heads), (CF_LE_W(bs.heads) != 1) ? "s" : "",
	     CF_LE_W(bs.secs_track), (CF_LE_W(bs.secs_track) != 1) ? "s" : "");
      printf("logical sector size is %d,\n",sector_size);
      printf("using 0x%02x media descriptor, with %d sectors;\n",
	     (int) (bs.media), num_sectors);
      printf("file system has %d %d-bit FAT%s and %d sector%s per cluster.\n",
	     (int) (bs.fats), size_fat, (bs.fats != 1) ? "s" : "",
	     (int) (bs.cluster_size), (bs.cluster_size != 1) ? "s" : "");
      printf ("FAT size is %d sector%s, and provides %d cluster%s.\n",
	      fat_length, (fat_length != 1) ? "s" : "",
	      cluster_count, (cluster_count != 1) ? "s" : "");
      if (size_fat != 32)
	printf ("Root directory contains %d slots.\n",
		(int) (bs.dir_entries[0]) + (int) (bs.dir_entries[1]) * 256);
      printf ("Volume ID is %08lx, ", volume_id &
	      (atari_format ? 0x00ffffff : 0xffffffff));
      if ( strcmp(volume_name, "           ") )
	printf("volume label %s.\n", volume_name);
      else
	printf("no volume label.\n");
    }

  /* Make the file allocation tables! */

  if ((fat = (unsigned char *) malloc (fat_length * sector_size)) == NULL)
    die ("unable to allocate space for FAT image in memory");

  memset( fat, 0, fat_length * sector_size );

  mark_FAT_cluster (0, 0xffffffff);	/* Initial fat entries */
  mark_FAT_cluster (1, 0xffffffff);
  fat[0] = (unsigned char) bs.media;	/* Put media type in first byte! */
  if (size_fat == 32) {
    /* Mark cluster 2 as EOF (used for root dir) */
    mark_FAT_cluster (2, FAT_EOF);
  }

  /* Make the root directory entries */

  size_root_dir = (size_fat == 32) ?
		  bs.cluster_size*sector_size :
		  (((int)bs.dir_entries[1]*256+(int)bs.dir_entries[0]) *
		   sizeof (struct msdos_dir_entry));
  if ((root_dir = (struct msdos_dir_entry *) malloc (size_root_dir)) == NULL)
    {
      free (fat);		/* Tidy up before we die! */
      die ("unable to allocate space for root directory in memory");
    }

  memset(root_dir, 0, size_root_dir);
  if ( memcmp(volume_name, "           ", 11) )
    {
      struct msdos_dir_entry *de = &root_dir[0];
      memcpy(de->name, volume_name, 11);
      de->attr = ATTR_VOLUME;
      ctime = localtime(&create_time);
      de->time = CT_LE_W((unsigned short)((ctime->tm_sec >> 1) +
			  (ctime->tm_min << 5) + (ctime->tm_hour << 11)));
      de->date = CT_LE_W((unsigned short)(ctime->tm_mday +
					  ((ctime->tm_mon+1) << 5) +
					  ((ctime->tm_year-80) << 9)));
      de->ctime_ms = 0;
      de->ctime = de->time;
      de->cdate = de->date;
      de->adate = de->date;
      de->starthi = CT_LE_W(0);
      de->start = CT_LE_W(0);
      de->size = CT_LE_L(0);
    }

  if (size_fat == 32) {
    /* For FAT32, create an info sector */
    struct fat32_fsinfo *info;

    if (!(info_sector = malloc( sector_size )))
      die("Out of memory");
    memset(info_sector, 0, sector_size);
    /* fsinfo structure is at offset 0x1e0 in info sector by observation */
    info = (struct fat32_fsinfo *)(info_sector + 0x1e0);

    /* Info sector magic */
    info_sector[0] = 'R';
    info_sector[1] = 'R';
    info_sector[2] = 'a';
    info_sector[3] = 'A';

    /* Magic for fsinfo structure */
    info->signature = CT_LE_L(0x61417272);
    /* We've allocated cluster 2 for the root dir. */
    info->free_clusters = CT_LE_L(cluster_count - 1);
    info->next_cluster = CT_LE_L(2);

    /* Info sector also must have boot sign */
    *(__u16 *)(info_sector + 0x1fe) = CT_LE_W(BOOT_SIGN);
  }

  if (!(blank_sector = malloc( sector_size )))
      die( "Out of memory" );
  memset(blank_sector, 0, sector_size);
}


/* Write the new filesystem's data tables to wherever they're going to end up! */

#define error(str)				\
  do {						\
    free (fat);					\
    if (info_sector) free (info_sector);	\
    free (root_dir);				\
    die (str);					\
  } while(0)

#define seekto(pos,errstr)						\
  do {									\
    loff_t __pos = (pos);						\
    if (llseek (dev, __pos, SEEK_SET) != __pos)				\
	error ("seek to " errstr " failed whilst writing tables");	\
  } while(0)

#define writebuf(buf,size,errstr)			\
  do {							\
    int __size = (size);				\
    if (write (dev, buf, __size) != __size)		\
	error ("failed whilst writing " errstr);	\
  } while(0)


void
write_tables (void)
{
  int x;
  int fat_length;
#ifdef _WIN32
  int blk;
#endif

  fat_length = (size_fat == 32) ?
	       CF_LE_L(bs.fat32.fat32_length) : CF_LE_W(bs.fat_length);

  seekto( 0, "start of device" );
  /* clear all reserved sectors */
  for( x = 0; x < reserved_sectors; ++x )
    writebuf( blank_sector, sector_size, "reserved sector" );
  /* seek back to sector 0 and write the boot sector */
  seekto( 0, "boot sector" );
  writebuf( (char *) &bs, sizeof (struct msdos_boot_sector), "boot sector" );
  /* on FAT32, write the info sector and backup boot sector */
  if (size_fat == 32)
    {
      seekto( CF_LE_W(bs.fat32.info_sector)*sector_size, "info sector" );
      writebuf( info_sector, 512, "info sector" );
      if (backup_boot != 0)
	{
	  seekto( backup_boot*sector_size, "backup boot sector" );
	  writebuf( (char *) &bs, sizeof (struct msdos_boot_sector),
		    "backup boot sector" );
	}
    }
  /* seek to start of FATS and write them all */
  seekto( reserved_sectors*sector_size, "first FAT" );
  for (x = 1; x <= nr_fats; x++)
#ifdef _WIN32
	  /*
	   * WIN32 appearently has problems writing very large chunks directly
	   * to disk devices. To not produce errors because of resource shortages
	   * split up the write in sector size chunks.
	   */
	  for (blk = 0; blk < fat_length; blk++)
		  writebuf(fat+blk*sector_size, sector_size, "FAT");
#else
    writebuf( fat, fat_length * sector_size, "FAT" );
#endif
  /* Write the root directory directly after the last FAT. This is the root
   * dir area on FAT12/16, and the first cluster on FAT32. */
  writebuf( (char *) root_dir, size_root_dir, "root directory" );

  if (blank_sector) free( blank_sector );
  if (info_sector) free( info_sector );
  free (root_dir);   /* Free up the root directory space from setup_tables */
  free (fat);  /* Free up the fat table space reserved during setup_tables */
}


/* Report the command usage and return a failure error code */

void
usage (void)
{
  fatal_error("\
Usage: mkdosfs [-A] [-c] [-C] [-v] [-I] [-l bad-block-file] [-b backup-boot-sector]\n\
       [-m boot-msg-file] [-n volume-name] [-i volume-id]\n\
       [-s sectors-per-cluster] [-S logical-sector-size] [-f number-of-FATs]\n\
       [-h hidden-sectors] [-F fat-size] [-r root-dir-entries] [-R reserved-sectors]\n\
       /dev/name [blocks]\n");
}

/*
 * ++roman: On m68k, check if this is an Atari; if yes, turn on Atari variant
 * of MS-DOS filesystem by default.
 */
void check_atari( void )
{
#ifdef __mc68000__
    FILE *f;
    char line[128], *p;

    if (!(f = fopen( "/proc/hardware", "r" ))) {
	perror( "/proc/hardware" );
	return;
    }

    while( fgets( line, sizeof(line), f ) ) {
	if (strncmp( line, "Model:", 6 ) == 0) {
	    p = line + 6;
	    p += strspn( p, " \t" );
	    if (strncmp( p, "Atari ", 6 ) == 0)
		atari_format = 1;
	    break;
	}
    }
    fclose( f );
#endif
}

#ifdef __cplusplus
};
#endif

/* That's All Folks */
/* Local Variables: */
/* tab-width: 8     */
/* End:             */
