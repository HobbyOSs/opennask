#include "../fatlib.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#pragma clang diagnostic ignored "-Wbitwise-op-parentheses"

#ifdef DDI
DRIVE *open_image(char *name);
#else
#define open_image(N) fat_open_image(N,0)
#endif

#define STATS 1
#define DOSLIKE 4
#define UNIXLIKE 8
#define RECURSIVE 16
#define CLUSTER 32

static void print_short(DRIVE *dv, char *t, int n)
{
	char buf[3*8], *d = buf;
	do {
		unsigned u = *(unsigned char*)t++;
		if(u>=0x80) u = fat_cp_to_u(dv, u);
		d = fat_put_utf8(d, buf+3*8, u);
	} while(--n);
	fwrite(buf, 1, d-buf, stdout);
}

DRIVE *dv;
char path[512];
int flags = DOSLIKE;
unsigned files, dirs, size;

int list(int l)
{
	FDIR *dir;
	struct fat_dirent de;
	int v;

	path[l] = 0;
	dir = fat_opendir(dv, path);
	if(!dir) return -1;
	for(;;) {
		v = fat_readdir(dir, &de);
		if(v<=0) break;
		if(flags&RECURSIVE && de.name[0]=='.')
			continue;
		if(flags&CLUSTER)
			printf("%6u ", de.cluster);
		if(flags&DOSLIKE) {
			print_short(dv, de.sname, 8);
			putchar(' ');
			print_short(dv, de.sname+8, 3);
			if(de.attr & 0x10) printf(" <DIR>     ");
			else printf(" %10u", de.size);
			printf(" %4u-%02u-%02u", FAT_GET_YEAR(de.mtime), FAT_GET_MONTH(de.mtime), FAT_GET_DAY(de.mtime));
			printf("  %2u:%02u  ", FAT_GET_HOUR(de.mtime), FAT_GET_MIN(de.mtime));
		} else if(flags&UNIXLIKE) {
			int m = 0666;
			if(de.attr&0x10) m |= 01111;
			if(de.attr&0x01) m &= ~0222;
			{
				char a[10];
				int i;
				for(i=10; --i>=0; m>>=1)
					a[i] = m&1 ? "drwxrwxrwx"[i] : '-';
				fwrite(a, 1, 10, stdout);
			}
			printf("   1 0        0        %8d ", de.size);
			printf("%02u-%02u-%02u", FAT_GET_MONTH(de.mtime), FAT_GET_DAY(de.mtime), FAT_GET_YEAR(de.mtime)%100);
			printf("  %2u:%02u ", FAT_GET_HOUR(de.mtime), FAT_GET_MIN(de.mtime));
		}
		if(flags&RECURSIVE)
			fwrite(path, 1, l, stdout);
		printf("%s\n", de.name);

		if(flags&RECURSIVE & de.attr&0x10) {
			if(l+de.nlen+2 >= sizeof path) errx(1, "%s/%s: Path too long", path, de.name);
			memcpy(path+l, de.name, de.nlen);
			path[l+de.nlen] = '/';
			list(l+de.nlen+1);
		}
		if(de.attr&0x10) dirs++;
		else {files++; size += de.size;}
	}
	fat_closedir(dir);
	return v;
}

int main(int argc, char *argv[])
{
	struct fat_dstat ds;
	char *img = 0;
	int tty;
	int o, l;

	while((o = getopt(argc, argv, "i:suc1R"))>=0) switch(o) {
		case 'i': img = optarg; break;
		case 's': flags|=STATS; break;
		case 'u': flags=flags&~DOSLIKE|UNIXLIKE; break;
		case '1': flags&=~(DOSLIKE|UNIXLIKE); break;
		case 'c': flags|=CLUSTER; break;
		case 'R': flags|=RECURSIVE; break;
		case '?':
			printf("fdir [-u1cR] "
#ifdef FLOPPY_DEFAULT
				"[-i image]"
#else
				"-i image"
#endif
			"\n");
			return 1;
	}
	if(!img) {
		img = getenv("FLOPPY");
		if(!img)
#ifdef FLOPPY_DEFAULT
			img = FLOPPY_DEFAULT;
#else
			errx(1, "No image (-i)");
#endif
	}

	if(argc>optind+1) {
		printf("usage: fdir [-su1cR] [-i image] [dir]\n");
		return 1;
	}

	l = 0;
	if(argc==optind+1) {
		char *p = argv[optind];
		if(*p=='/') p++;
		l = strlen(p);
		if(l>510) errx(1, "%s: Path too long", argv[optind]);
		if(l && p[l-1]=='/') l--;
		memcpy(path, p, l);
	}
	path[l++] = '/';

	dv = open_image(img);
	if(!dv) err(1, "%s", img);
	fat_set_cp(dv, fat_cp852);

	tty = isatty(1);
	if(tty) printf("\33%%G"); /* utf-8 */
	if(flags&STATS) {
		if(fat_dstat(dv, &ds, 1)<0)
			err(1, "drive stat");
		printf("\n Volume ");
		if(strcmp(ds.label, "NO NAME")) printf("is %s\n", ds.label);
		else printf("has no label\n");
		printf(" Volume Serial Number is %04X:%04X\n",
			(int)ds.serial>>16, (int)ds.serial&0xFFFF);
		printf(" Directory %s\n\n", path);
	}
	o = list(l);
	if(tty) printf("\33%%@");
	if(o) err(1, "%s:%.*s", img, l, path);

	if(flags & STATS)
		printf( "%9u file%c         %12u bytes\n"
			"%9u director%-3s   %12lu bytes free\n",
			files, "s "[files==1], size,
			dirs, dirs==1?"y":"ies", ds.free << ds.sector_sh);
	return 0;
}
