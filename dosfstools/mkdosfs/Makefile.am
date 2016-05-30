# LIB
noinst_LIBRARIES     = libmkdosfs.a
libmkdosfs_a_SOURCES = mkdosfs.c mkdosfs.h

# PROGRAM
bin_PROGRAMS         = mkdosfs
mkdosfs_SOURCES      = main.c
mkdosfs_LDADD        = libmkdosfs.a
mkdosfs_CFLAGS       = -I./
