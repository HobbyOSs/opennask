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
#include "mkdosfs.c"

/* The "main" entry point into the utility - we pick up the options and attempt to process them in some sort of sensible
   way.  In the event that some/all of the options are invalid we need to tell the user so that something can be done! */

int
main (int argc, char **argv)
{
  int c;
  char *tmp;
  char *listfile = NULL;
  FILE *msgfile;
#ifdef _WIN32
  static char dev_buf[] = "\\\\.\\X:";
#else
  struct stat statbuf;
#endif
  int i = 0, pos, ch;
  int create = 0;
  unsigned long long cblocks;

  if (argc && *argv) {		/* What's the program name? */
    char *p;
    program_name = *argv;
#ifdef _WIN32
    if ((p = strrchr( program_name, '\\' )))
#else
    if ((p = strrchr( program_name, '/' )))
#endif
	program_name = p+1;
  }

  time(&create_time);
  volume_id = (long)create_time;	/* Default volume ID = creation time */
  check_atari();

  printf ("%s " VERSION " (" VERSION_DATE ")\n"
#ifdef _WIN32
	  "Win32 port by Jens-Uwe Mager <jum@anubis.han.de>\n"
#endif
	   , program_name);

  while ((c = getopt (argc, argv, "AbcCf:F:Ii:l:m:n:r:R:s:S:h:v")) != EOF)
    /* Scan the command line for options */
    switch (c)
      {
      case 'A':		/* toggle Atari format */
	atari_format = !atari_format;
	break;

      case 'b':		/* b : location of backup boot sector */
	backup_boot = (int) strtol (optarg, &tmp, 0);
	if (*tmp || backup_boot < 2 || backup_boot > 0xffff)
	  {
	    printf ("Bad location for backup boot sector : %s\n", optarg);
	    usage ();
	  }
	break;

      case 'c':		/* c : Check FS as we build it */
	check = TRUE;
	break;

      case 'C':		/* C : Create a new file */
	create = TRUE;
	break;

      case 'f':		/* f : Choose number of FATs */
	nr_fats = (int) strtol (optarg, &tmp, 0);
	if (*tmp || nr_fats < 1 || nr_fats > 4)
	  {
	    printf ("Bad number of FATs : %s\n", optarg);
	    usage ();
	  }
	break;

      case 'F':		/* F : Choose FAT size */
	size_fat = (int) strtol (optarg, &tmp, 0);
	if (*tmp || (size_fat != 12 && size_fat != 16 && size_fat != 32))
	  {
	    printf ("Bad FAT type : %s\n", optarg);
	    usage ();
	  }
	size_fat_by_user = 1;
	break;

      case 'h':        /* h : number of hidden sectors */
	hidden_sectors = (int) strtol (optarg, &tmp, 0);
	if ( *tmp || hidden_sectors < 0 )
	  {
	    printf("Bad number of hidden sectors : %s\n", optarg);
	    usage ();
	  }
	break;

      case 'I':
	ignore_full_disk = 1;
	break;

      case 'i':		/* i : specify volume ID */
	volume_id = strtoul(optarg, &tmp, 16);
	if ( *tmp )
	  {
	    printf("Volume ID must be a hexadecimal number\n");
	    usage();
	  }
	break;

      case 'l':		/* l : Bad block filename */
	listfile = optarg;
	break;

      case 'm':		/* m : Set boot message */
	if ( strcmp(optarg, "-") )
	  {
	    msgfile = fopen(optarg, "r");
	    if ( !msgfile )
	      perror(optarg);
	  }
	else
	  msgfile = stdin;

	if ( msgfile )
	  {
	    /* The boot code ends at offset 448 and needs a null terminator */
	    i = MESSAGE_OFFSET;
	    pos = 0;		/* We are at beginning of line */
	    do
	      {
		ch = getc(msgfile);
		switch (ch)
		  {
		  case '\r':	/* Ignore CRs */
		  case '\0':	/* and nulls */
		    break;

		  case '\n':	/* LF -> CR+LF if necessary */
		    if ( pos )	/* If not at beginning of line */
		      {
			dummy_boot_code[i++] = '\r';
			pos = 0;
		      }
		    dummy_boot_code[i++] = '\n';
		    break;

		  case '\t':	/* Expand tabs */
		    do
		      {
			dummy_boot_code[i++] = ' ';
			pos++;
		      }
		    while ( pos % 8 && i < BOOTCODE_SIZE-1 );
		    break;

		  case EOF:
		    dummy_boot_code[i++] = '\0'; /* Null terminator */
		    break;

		  default:
		    dummy_boot_code[i++] = ch; /* Store character */
		    pos++;	/* Advance position */
		    break;
		  }
	      }
	    while ( ch != EOF && i < BOOTCODE_SIZE-1 );

	    /* Fill up with zeros */
	    while( i < BOOTCODE_SIZE-1 )
		dummy_boot_code[i++] = '\0';
	    dummy_boot_code[BOOTCODE_SIZE-1] = '\0'; /* Just in case */

	    if ( ch != EOF )
	      printf ("Warning: message too long; truncated\n");

	    if ( msgfile != stdin )
	      fclose(msgfile);
	  }
	break;

      case 'n':		/* n : Volume name */
	sprintf(volume_name, "%-11.11s", optarg);
	break;

      case 'r':		/* r : Root directory entries */
	root_dir_entries = (int) strtol (optarg, &tmp, 0);
	if (*tmp || root_dir_entries < 16 || root_dir_entries > 32768)
	  {
	    printf ("Bad number of root directory entries : %s\n", optarg);
	    usage ();
	  }
	break;

      case 'R':		/* R : number of reserved sectors */
	reserved_sectors = (int) strtol (optarg, &tmp, 0);
	if (*tmp || reserved_sectors < 1 || reserved_sectors > 0xffff)
	  {
	    printf ("Bad number of reserved sectors : %s\n", optarg);
	    usage ();
	  }
	break;

      case 's':		/* s : Sectors per cluster */
	sectors_per_cluster = (int) strtol (optarg, &tmp, 0);
	if (*tmp || (sectors_per_cluster != 1 && sectors_per_cluster != 2
		     && sectors_per_cluster != 4 && sectors_per_cluster != 8
		   && sectors_per_cluster != 16 && sectors_per_cluster != 32
		&& sectors_per_cluster != 64 && sectors_per_cluster != 128))
	  {
	    printf ("Bad number of sectors per cluster : %s\n", optarg);
	    usage ();
	  }
	break;

      case 'S':		/* S : Sector size */
	sector_size = (int) strtol (optarg, &tmp, 0);
	if (*tmp || (sector_size != 512 && sector_size != 1024 &&
		     sector_size != 2048 && sector_size != 4096 &&
		     sector_size != 8192 && sector_size != 16384 &&
		     sector_size != 32768))
	  {
	    printf ("Bad logical sector size : %s\n", optarg);
	    usage ();
	  }
	sector_size_set = 1;
	break;

      case 'v':		/* v : Verbose execution */
	++verbose;
	break;

      default:
	printf( "Unknown option: %c\n", c );
	usage ();
      }
  if (optind < argc)
    {
      device_name = argv[optind];  /* Determine the number of blocks in the FS */
#ifdef _WIN32
      if (device_name[1] == ':' && device_name[2] == '\0') {
	  dev_buf[4] = device_name[0];
	  device_name = dev_buf;
	  is_device = 1;
      }
#endif
      if (!create)
         cblocks = count_blocks (device_name); /*  Have a look and see! */
    }
  if (optind == argc - 2)	/*  Either check the user specified number */
    {
      blocks = strtoull (argv[optind + 1], &tmp, 0);
      if (!create && blocks != cblocks)
	{
	  fprintf (stderr, "Warning: block count mismatch: ");
	  fprintf (stderr, "found %llu but assuming %llu.\n",cblocks,blocks);
	}
    }
  else if (optind == argc - 1)	/*  Or use value found */
    {
      if (create)
	die( "Need intended size with -C." );
      blocks = cblocks;
      tmp = "";
    }
  else
    {
      fprintf (stderr, "No device specified!\n");
      usage ();
    }
  if (*tmp)
    {
      printf ("Bad block count : %s\n", argv[optind + 1]);
      usage ();
    }

  if (check && listfile)	/* Auto and specified bad block handling are mutually */
    die ("-c and -l are incompatible");		/* exclusive of each other! */

  if (!create) {
    check_mount (device_name);	/* Is the device already mounted? */
    dev = open (device_name, O_RDWR|O_SHARED);	/* Is it a suitable device to build the FS on? */
    if (dev < 0)
      die ("unable to open %s");
#ifdef _WIN32
	if (is_device) {
		if (fsctl(dev, FSCTL_LOCK_VOLUME) == -1)
			die("unable to lock %s");
	}
#endif
  }
  else {
      off_t offset = blocks*BLOCK_SIZE - 1;
      char null = 0;
      /* create the file */
      dev = open( device_name, O_RDWR|O_CREAT|O_TRUNC, 0666 );
      if (dev < 0)
	die("unable to create %s");
      /* seek to the intended end-1, and write one byte. this creates a
       * sparse-as-possible file of appropriate size. */
      if (llseek( dev, offset, SEEK_SET ) != offset)
	die( "seek failed" );
      if (write( dev, &null, 1 ) < 0)
	die( "write failed" );
      if (llseek( dev, 0, SEEK_SET ) != 0)
	die( "seek failed" );
  }

#ifdef _WIN32
  if (!is_device)
	  check = 0;
  establish_params();
#else
  if (fstat (dev, &statbuf) < 0)
    die ("unable to stat %s");
  if (!S_ISBLK (statbuf.st_mode)) {
    statbuf.st_rdev = 0;
    check = 0;
  }
  else
    /*
     * Ignore any 'full' fixed disk devices, if -I is not given.
     * On a MO-disk one doesn't need partitions.  The filesytem can go
     * directly to the whole disk.  Under other OSes this is known as
     * the 'superfloppy' format.  As I don't know how to find out if
     * this is a MO disk I introduce a -I (ignore) switch.  -Joey
     */
    if (!ignore_full_disk && (
	(statbuf.st_rdev & 0xff3f) == 0x0300 || /* hda, hdb */
	(statbuf.st_rdev & 0xff0f) == 0x0800 || /* sd */
	(statbuf.st_rdev & 0xff3f) == 0x0d00 || /* xd */
	(statbuf.st_rdev & 0xff3f) == 0x1600 )  /* hdc, hdd */
	)
      die ("Will not try to make filesystem on full-disk device '%s' (use -I if wanted)");

  establish_params (statbuf.st_rdev,statbuf.st_size);
                                /* Establish the media parameters */
#endif

  setup_tables (sectors_per_cluster);	/* Establish the file system tables */

  if (check)			/* Determine any bad block locations and mark them */
    check_blocks ();
  else if (listfile)
    get_list_blocks (listfile);

  write_tables ();		/* Write the file system tables away! */

#ifdef _WIN32
	if (is_device) {
		if (fsctl(dev, FSCTL_DISMOUNT_VOLUME) == -1)
			die("unable to dismount %s");
		if (fsctl(dev, FSCTL_UNLOCK_VOLUME) == -1)
			die("unable to unlock %s");
	}
#endif
  exit (0);			/* Terminate with no errors! */
}
