AM_CPPFLAGS     = -DDDI

noinst_PROGRAMS = fdstat fdir fget fput

fdstat_SOURCES  = fdstat.c ddi.c
fdstat_CFLAGS   = -I$(top_builddir)
fdstat_LDADD    = $(top_builddir)/libfat.a

fdir_SOURCES	= fdir.c ddi.c
fdir_CFLAGS     = -I$(top_builddir)
fdir_LDADD      = $(top_builddir)/libfat.a

fget_SOURCES	= fget.c ddi.c
fget_CFLAGS     = -I$(top_builddir)
fget_LDADD      = $(top_builddir)/libfat.a

fput_SOURCES	= fput.c
fput_CFLAGS     = -I$(top_builddir)
fput_LDADD      = $(top_builddir)/libfat.a
