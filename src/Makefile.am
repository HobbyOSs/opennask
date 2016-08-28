# main program
bin_PROGRAMS	  = opennask
opennask_SOURCES  = main.cpp         \
		    nask_utility.cpp \
		    nask_utility.hpp \
		    nask_defs.hpp    \
		    tinyexpr.c	     \
		    tinyexpr.h


opennask_CXXFLAGS = -I$(top_builddir)/parasol \
                    -I$(top_builddir)/fatlib

opennask_LDADD    = $(top_builddir)/parasol/libparasol.a \
		    $(top_builddir)/fatlib/libfat.a -lpthread

if BUILD_DOSFSTOOLS
# Add CXXFLAGS
opennask_CXXFLAGS += -I$(top_builddir)/dosfstools/mkdosfs
opennask_LDADD    += $(top_builddir)/dosfstools/mkdosfs/libmkdosfs.a

endif
