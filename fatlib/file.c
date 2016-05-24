/*
 *	Copyright (C) 2004 Jan Bobrowski <jb@wizard.ae.krakow.pl>
 *
 *	This library is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU Lesser General Public
 *	License version 2.1 as published by the Free Software Foundation.
 */

#include "lib.h"
#include <string.h>

struct file *file_alloc(DRIVE *drive)
{
	struct file *file = ALLOC(struct file);
	if(file) {
		file->drive = drive;
		file->begin = file->end = 0;
		file->size = 0;
		file->count = 1;
		file->dir = 0;
		list_init(&file->childs);
		list_init(&file->dirlst);
		list_init(&file->dirty);
		dget(drive);
	}
	return file;
}

int file_truncate(struct file *file)
{
	clust_t c;
	if(file->begin) {
		if(FRAG_IS(FIXED, file->begin))
			return -EPERM;
		file->begin = file->end = frag_free_chain(file->begin);
	}
	c = file_first_cluster(file);
	if(c) {
		fat_free_chain(file->drive, c);
		file->clust = 0;
	}
	return 0;
}

void file_free(struct file *file)
{
	assert(list_empty(&file->childs));
	DBG("file_free: %p dir:%p%s%s\n", file, file->dir, file_is_dirty(file)?" (modified)":"", file_is_deleted(file)?" (deleted)":"");

	if(file_is_deleted(file)) {
		file_truncate(file);
		list_del(&file->dirty);
	} else {
		if(file_is_dirty(file))
			file_write_meta(file);
	
		if(file->begin)
			file->begin = file->end = frag_free_chain(file->begin);
		if(file->dir) {
			list_del_init(&file->dirlst);
			fput(file->dir);
		} else {
			assert(file->drive->root == file);
			file->drive->root = 0;
		}
	}
	dput(file->drive);
	FREE(file);
}

struct file *drive_get_root(DRIVE *dv)
{
	struct file *root;
	if(dv->root)
		return fget(dv->root);
	
	root = file_alloc(dv);
	if(!root) return root;

	if((dv->flags&DRV_FAT) != DRV_FAT32) {
		struct frag *frag = frag_alloc();
		assert(frag);
		root->clust = 0;
		frag->fpos = 0;
		frag->dpos = dv->root_start<<SECSH;
		frag->len = dv->root_ent<<5;
		frag->flags = FRAG_WHOLE|FRAG_LAST|FRAG_FIXED;
		root->begin = root->end = frag;
		DBG_FRAG("root", frag);
	} else
		root->clust = dv->root_clust;
	root->entry = 0;
	root->nlfn = 0;
	dv->root = root;
	return root;
}

/* io */

static struct frag *file_next_frag(struct file *file, struct frag *frag)
{
	assert(FRAG_IS(WHOLE, frag));
	assert(!FRAG_IS(LAST, frag));
	if(file->end == frag) {
		struct frag *next = frag_at(file->drive, frag->nxclust);
		if(!next) return next; /* ENOMEM */
		frag->next = next;
		next->fpos = frag_fend(frag);
		file->end = next;
	}
	return frag->next;
}

int file_io(struct file *file, struct io *io)
{
	struct frag *frag;
	int oldlen, grow = 0;

	io->drive = file->drive;
	io->err = 0;

	if(io->op != IOOP_READ) { /* XXX here? */
		int ret = will_write(file->drive);
		if(ret) return ret;
	}

	frag = file->end;
	if(!frag) {
		clust_t c = file_first_cluster(file);
		if(!c) {
			if(io->pos) return -ESPIPE; /* io behind end */
			goto newfrag;
		}
		frag = frag_at(io->drive, c);
		if(!frag) return -ENOMEM;
		frag->fpos = 0;
		file->begin = file->end = frag;
	}

	/* find file fragment */

	if(io->pos < frag->fpos)
		frag = file->begin;

	for(;;) {
		int fre = frag_fend(frag);
		if(io->pos < fre) break;
		if(!FRAG_IS(WHOLE, frag) && frag_more(io->drive, frag))
			continue;
		assert(FRAG_IS(WHOLE, frag));
		if(FRAG_IS(BAD, frag))
			return -EIO;
		if(FRAG_IS(LAST, frag)) {
			if(io->pos != fre)
				return -ESPIPE; /* io behind end */
			if(io->op == IOOP_READ)
				return 0;
			if(FRAG_IS(FIXED, frag))
				return -EFBIG;
			oldlen = frag->len;
			grow = frag_expand(frag, io);
			if(grow)
				goto doio;
			/* can't write at the end of frag */
			assert(io->pos == frag_fend(frag));
			goto newfrag;
		}
		frag = file_next_frag(file, frag);
		if(!frag) return -ENOMEM;
	}

	/* expand fragment */

	for(;;) {
		int ioe = io->pos + io->len;
		int fre = frag_fend(frag);
		if(ioe <= fre) break;
		if(!FRAG_IS(WHOLE, frag) && frag_more(io->drive, frag))
			continue;
		assert(FRAG_IS(WHOLE, frag));
		if(io->op!=IOOP_READ && frag->flags == (FRAG_WHOLE|FRAG_LAST)) {
			oldlen = frag->len;
			grow = frag_expand(frag, io);
		}
		break;
	}

	/* io */
doio:
	for(;;) {
		int fullio;

		if(frag->len == 0) {
			assert(FRAG_IS(BAD, frag));
			return -EIO;
		}
		if(grow) {
			assert(io->op!=IOOP_READ);
			io->op |= IOOP_GROW;
		}

		fullio = frag_io(frag, io);
		if(io->op!=IOOP_READ) file_modified(file);

		if(grow) {
			clust_t pvc, c;
			int n = need_clust(io->drive, io->pos - frag->fpos)
				<< io->drive->clust_sh;
			if(n <= oldlen) {
				if(!oldlen) {
					assert(file->end != frag);
					FREE(frag);
				}
				return io->err;
			}
			
			/* file grown */
			
			frag->len = n;
			if(oldlen) {
				c = pos_to_clust(io->drive, frag->dpos + oldlen);
				pvc = c-1; 
			} else {
				c = frag_first_clust(io->drive, frag);
				if(file->end) {
					struct frag *end = file->end;
					assert(frag_fend(end) == frag->fpos);
					end->next = frag;
					end->flags = FRAG_WHOLE;
					end->nxclust = c;
					pvc = frag_last_clust(io->drive, end);
				} else {
					assert(!file->clust);
					file->begin = frag;
					file->clust = c;
					pvc = 0;
				}
				file->end = frag;
				assert(frag->flags = FRAG_WHOLE|FRAG_LAST);
			}
			n = frag->len-oldlen >> io->drive->clust_sh;
			fat_write_chain(io->drive, pvc, c, n, FAT_EOF);
		}

		if(!io->len || !fullio) return io->err;

		if(!FRAG_IS(LAST, frag)) {
			frag = file_next_frag(file, frag);
			if(!frag) return -ENOMEM;
			continue;
		}
newfrag:
		if(io->op==IOOP_READ) return 0;
		frag = frag_alloc_new(io); /* err in io->err */
		if(!frag) return io->err;
		frag->fpos = io->pos;
		grow=1; oldlen=0;
	}
}

/* lookup */

int lookup(DRIVE *dv, struct file **fptr, const char *path, int flags)
{
	struct file *dir = *fptr;
	int err = 0;

	if(path[0] == '/') {
		path++;
		if(dir) fput(dir);
		dir = 0;
	}
	if(!dir)
		dir = drive_get_root(dv);

	for(;;) {
		char *p = strchr(path, '/');
		if(!p) break;
		if(p==path) goto notfound;
		err = lookup_one(&dir, path, p-path, LOOK_DIR);
		if(err) goto error;
		path = p+1;
	}
	if(*path != 0) {
		err = lookup_one(&dir, path, strlen(path), flags);
		if(err) goto error;
	}
	*fptr = dir;
	return 0;
notfound:
	err = -ENOENT;
error:
	fput(dir);
	return err;
}
