#include "../fatlib.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#if 1
DRIVE *open_image(char *name);
#else
#define open_image(N) fat_open_image(N,0)
#endif

int main(int argc, char *argv[])
{
	char *img = 0;
	DRIVE *dv;
	FILE *src;
	int o;

	while((o = getopt(argc, argv, "i:"))>=0) switch(o) {
		case 'i': img = optarg; break;
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
		printf("usage: fget [-i image] file\n");
		return 1;
	}

	dv = open_image(img);
	if(!dv) err(1, "%s", img);
	fat_set_cp(dv, fat_cp852);

	src = fat_fopen(dv, argv[optind], "r");
	if(!src) err(1, "%s", argv[optind]);
	
	for(;;) {
		static char buf[8128];
		int v, w;
		v = fread(buf, 1, sizeof buf, src);
		if(v>0) {
			w = fwrite(buf, 1, v, stdout);
			if(w < v)
				err(1, "fwrite");
		}
		if(v == sizeof buf) continue;
		if(ferror(src))
			err(1, "fread");
		break;
	}
	fclose(src);

	return 0;
}
