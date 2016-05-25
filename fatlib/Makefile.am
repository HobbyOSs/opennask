AUTOMAKE_OPTIONS  = foreign subdir-objects

noinst_LIBRARIES  = libfat.a
noinst_HEADERS    = fatlib.h lib.h list.h
libfat_a_SOURCES  = drive.c img.c buf.c fat.c file.c dir.c del.c stdio.c \
                    readdir.c seekdir.c opensubdir.c mkdir.c rmdir.c     \
                    delete.c dstat.c nocp.c cp.c cp850.c cp852.c

EXTRA_DIST        = examples
EXTRA_DATA        = $(wildcard examples/*) uni_up.inc libfat.sym README TODO

# examples
noinst_PROGRAMS = examples/fdstat examples/fdir examples/fget examples/fput

examples_fdstat_SOURCES  = examples/fdstat.c examples/ddi.c
examples_fdstat_CFLAGS   = -I$(top_builddir)/fatlib/
examples_fdstat_CPPFLAGS = -DDDI
examples_fdstat_LDADD    = libfat.a

examples_fdir_SOURCES	 = examples/fdir.c examples/ddi.c
examples_fdir_CFLAGS     = -I$(top_builddir)/fatlib/
examples_fdir_CPPFLAGS   = -DDDI
examples_fdir_LDADD      = libfat.a

examples_fget_SOURCES	 = examples/fget.c examples/ddi.c
examples_fget_CFLAGS     = -I$(top_builddir)/fatlib/
examples_fget_CPPFLAGS   = -DDDI
examples_fget_LDADD      = libfat.a

examples_fput_SOURCES	 = examples/fput.c
examples_fput_CFLAGS     = -I$(top_builddir)/fatlib/
examples_fput_CPPFLAGS   = -DDDI
examples_fput_LDADD      = libfat.a
