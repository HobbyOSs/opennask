## top directory

# Build in these directories:
# test/Makefile <-- I can't compile it :<
SUBDIRS= reflcpp

if USE_LIBXML2
	SUBDIRS += tools
endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = reflcpp.pc

#EXTRA_DIST=autogen.sh
