#
# OSASK suffix rules for Makefile
#

#
# Automake supports an include directive that can be used to include
# other Makefile fragments when automake is run.
# reference: http://www.gnu.org/software/automake/manual/html_node/Include.html
#
# Usage  : include $(srcdir)/file
#
# Example: include $(top_builddir)/rules.mk
#

#
# Set GO Tools path, $(EXEEXT) will be automatically put by automake
#
SUFFIXES = .ask .ias .3as

#
# Set your own tool path
# If you want to use nask.exe, copy it into directory
#
WINE		= /usr/bin/wine
WINE_NASK	= $(WINE) ~/.wine/drive_c/MinGW/msys/1.0/bin/nask.exe

# GAS2NASK	= $(top_builddir)/20GO/toolstdc/gas2nask$(EXEEXT)
# NASK		= $(top_builddir)/20GO/toolstdc/nask$(EXEEXT)
# LIBRARIAN	= $(top_builddir)/20GO/toolstdc/golib00w$(EXEEXT)
# ASKA		= $(top_builddir)/28GO/aska/aska$(EXEEXT)
# NASKCNV	= $(top_builddir)/20GO/toolstdc/naskcnv0$(EXEEXT)
