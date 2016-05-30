# PROGRAM
bin_PROGRAMS    = dosfsck
dosfsck_SOURCES = boot.c    \
		  check.c   \
		  common.c  \
		  dosfsck.c \
		  fat.c     \
		  file.c    \
		  io.c	    \
                  lfn.c

noinst_HEADERS  = io.h      \
		  file.h    \
		  lfn.h	    \
		  fat.h	    \
		  common.h  \
		  dosfsck.h \
		  boot.h    \
		  check.h   \
		  ../version.h

# LIB
noinst_LIBRARIES     = libdosfsck.a
libdosfsck_a_SOURCES = $(dosfsck_SOURCES)
