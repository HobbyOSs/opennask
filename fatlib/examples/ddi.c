/* it is supposed to be a driver for DiskDupe (DDI) images (ro) */

#include <sys/file.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include "../fatlib.h"

#pragma clang diagnostic ignored "-Wimplicit-function-declaration"
#pragma clang diagnostic ignored "-Wgnu-designator"

typedef unsigned char u8;
#define elemof(T) (sizeof T/sizeof*T)

	  /* 360 1.2 720 1.44 */
char fmt[] = {18, 30, 18, 36}; /* sec per clust */

unsigned start, bpc;
int fd;
int *tab;

static int im_read(void *priv, char *buf, long pos, int *len);

struct fat_dops dops = {
	read: im_read,
};

DRIVE *open_image(char *name)
{
	u8 buf[0x64];
	int v;
	
	v = fd = open(name, O_RDONLY);
	if(v<0) goto err;
	flock(fd, LOCK_EX);
	v = read(fd, buf, sizeof buf);
	if(v != sizeof buf) goto err;

	if(memcmp(buf, "IM", 2) == 0) {
		int i, n;

		n = buf[0xA] - 1;
		if(n<0 || n>=elemof(fmt) || !fmt[n])
			errx(1, "DDI format not recognized");
		start = fmt[n]<<8; /* should be a track size? */
		bpc = fmt[n]<<9;
		
		n = (start - 0x64) / 6;
		tab = (int*)malloc(n * sizeof tab[0]);
		
		for(i=0;i<n;i++) {
			v = read(fd, buf, 6);
			if(v != 6) goto err;
			v = 0;
			if(buf[0]==1) v = buf[1] | buf[2]<<8;
			tab[i] = v-1;
		}
	}
	return fat_open_virt(&dops, 0, 0);
err:
	if(v<0) err(1, "%s", name);
	errx(1, "%s: Broken image", name);
}

static int im_read(void *priv, char *buf, long pos, int *len)
{
	int v;
	if(!tab) {
		int ret;
		v = lseek(fd, pos, SEEK_SET);
		if(v < 0) goto err;
		v = read(fd, buf, *len);
		ret = 0;
		if(v < *len) {
			if(v>=0) ret = ENXIO; /* img truncated */
			else {
err:
				v = 0;
				ret = errno;
			}
		}
		*len = v;
		return ret;
	} else {
		int l = *len;
		int c = pos/bpc;
		int o = pos%bpc;
		do {
			int s = bpc - o;
			int i = tab[c];
			if(s>l) s=l;
			if(i<0)
				memset(buf, 0, s);
			else {
				v = lseek(fd, start + i*bpc + o, SEEK_SET);
				if(v<0) break;
				v = read(fd, buf, s);
				if(v<s) break;
			}
			buf += s; l -= s;
			o = 0; c++;
		} while(l);
		if(l) {
			*len -= l;
			return v<0 ? errno : ENXIO;
		}
		return 0;
	}
}
