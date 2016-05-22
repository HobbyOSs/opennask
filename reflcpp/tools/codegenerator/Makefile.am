
AM_CPPFLAGS = -I$(top_srcdir) $(LIBXML2_CFLAGS)

bin_PROGRAMS = codegenerator
codegenerator_SOURCES = \
	CodeGenerator.hpp \
	CodeGenerator.cpp
codegenerator_LDADD = $(top_builddir)/reflcpp/libreflcpp.la $(LIBXML2_LIBS)

EXTRA_DIST = cg.sh

install-exec-hook:
	cp -p cg.sh $(DESTDIR)$(bindir)
