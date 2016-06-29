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
CC1		= $(CC) -S -Wall

INTEL2GAS	= $(top_builddir)/intel2gas/intel2gas$(EXEEXT)
OPENNASK	= $(top_builddir)/src/opennask$(EXEEXT)

bootpack.gas : bootpack.c Makefile
	$(CC1) bootpack.c -o bootpack.gas

bootpack.nas : bootpack.gas Makefile
	$(INTEL2GAS) -g -o bootpack.nas bootpack.gas

bootpack.obj : bootpack.nas Makefile
	$(OPENNASK) bootpack.nas bootpack.obj
