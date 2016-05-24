#include "../fatlib.h"
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <errno.h>
#include <err.h>

int main(int argc, char *argv[])
{
	char *img = 0;
	DRIVE *dv;
	FILE *dst;
	char *mode = "w";
	char *name;
	int o;

	while((o = getopt(argc, argv, "admi:"))>=0) switch(o) {
		case 'i': img = optarg; break;
		case 'a': mode = "a"; break;
		case 'd': mode = 0; break;
		case 'm': mode = ""; break;
	}
	if(!img) {
		img = getenv("FLOPPY");
		if(!img)
#ifdef FLOPPY_DEFAULT
			img = FLOPPY_DEFAULT;
#else
			errx(1, "No image");
#endif
	}

	if(argc==optind) {
		printf("usage:	fput [-a] [-i image] file < src\n");
		printf("	fput [-d] file	(rm/rmdir)\n");
		printf("	fput [-m] dir	(mkdir)\n");
		return 1;
	}
	
	name = argv[optind];

	time(&fat_time);
	dv = fat_open_image(img, 1);
	if(!dv) err(1, "%s", img);
	fat_set_cp(dv, fat_cp852);

	if(!mode) {
		if(fat_delete(dv, name)<0 && (errno!=EISDIR || fat_rmdir(dv, name)<0))
			err(1, "%s", name);
		return 0;
	} else if(!*mode) {
		if(fat_mkdir(dv, name)<0)
			err(1, "%s", name);
		return 0;
	}

	dst = fat_fopen(dv, name, mode);
	if(!dst) err(1, "%s", name);
	
	for(;;) {
		static char buf[8128];
		int v, w;
		v = fread(buf, 1, sizeof buf, stdin);
		if(v>0) {
			w = fwrite(buf, 1, v, dst);
			if(w < v)
				err(1, "%s", name);
		}
		if(v == sizeof buf) continue;
		if(ferror(dst))
			err(1, "%s", name);
		break;
	}
	if(fclose(dst) == EOF)
		err(1, "%s", name);
	return 0;
}
