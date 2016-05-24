#include "../fatlib.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

#ifdef DDI
DRIVE *open_image(char *name);
#else
#define open_image(N) fat_open_image(N,0)
#endif

static void prv(char *name, unsigned long v, int sh)
{
	unsigned long vk = (v<<sh)>>10;
	unsigned vkd = (v<<sh & 0x3FF)/102;
	printf("%8s: %8lu sec %7lu.%u KB\n", name, v, vk, vkd);
}

int main(int argc, char *argv[])
{
	DRIVE *dv;
	struct fat_dstat st;
	char *img = 0;

	if(argc>2) {
		printf("usage: fdstat [image]\n");
		return 1;
	} else if(argc==2)
		img = argv[1];
	else {
		img = getenv("FLOPPY");
		if(!img)
#ifdef FLOPPY_DEFAULT
			img = FLOPPY_DEFAULT;
#else
			errx(1, "No image");
#endif
	}

	dv = open_image(img);
	if(!dv) err(1, "%s", img);
	fat_set_cp(dv, fat_cp852);
	if(fat_dstat(dv, &st, 3)<0)
		err(1, "fat_dstat");
	fat_drive_close(dv);

	printf("label: '%s', serial: %04X-%04X, oem id: ", st.label, (int)st.serial>>16, (int)st.serial&0xFFFF);
	{
		int i;
		for(i=0; i<8; i++) {
			unsigned char c = st.oemid[i];
			if(c>=' ' && c<='~') putchar(c);
			else printf("\\%o", c);
		}
	}
	printf("\nsector size: %u bytes, cluster size: %u sec (%u bytes)\n",
		1<<st.sector_sh, 1<<st.cluster_sh, 1<<(st.cluster_sh+st.sector_sh));

	prv("size", st.size, st.sector_sh);
	prv("capacity", st.capacity, st.sector_sh);
	prv("free", st.free, st.sector_sh);
	prv("used", st.capacity-st.free-st.bad, st.sector_sh);
	prv("bad", st.bad, st.sector_sh);

	return 0;
}
