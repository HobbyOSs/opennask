#
# Generated 2016年  6月  1日 水曜日 23:55:00 JST 
#

NASK = $(top_builddir)/src/opennask$(EXEEXT)
FDSTAT = $(top_builddir)/fatlib/examples/fdstat$(EXEEXT)

bin_PROGRAMS =  ipl.img
ipl_img_SOURCES = ipl.nas

ipl.img$(EXEEXT) : ipl.nas
		$(NASK) ipl.nas ipl.img
		$(FDSTAT) ipl.img

