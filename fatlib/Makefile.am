lib_LTLIBRARIES   = libfat.la
noinst_HEADERS    = fatlib.h lib.h list.h
libfat_la_SOURCES = drive.c img.c buf.c fat.c file.c dir.c del.c stdio.c \
                    readdir.c seekdir.c opensubdir.c mkdir.c rmdir.c     \
                    delete.c dstat.c nocp.c cp.c cp850.c cp852.c

libfat_la_LDFLAGS = -no-undefined -version-info 0:0:0

EXTRA_DIST        = examples
EXTRA_DATA        = $(wildcard examples/*) uni_up.inc libfat.sym README TODO
