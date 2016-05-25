#
# Generated 2016年  5月 25日 水曜日 14:16:34 JST 
#

NASK = $(top_builddir)/src/opennask$(EXEEXT)
FDSTAT = $(top_builddir)/fatlib/examples/fdstat$(EXEEXT)

bin_PROGRAMS =  ipl.img
ipl_img_SOURCES = ipl.nas

ipl.img$(EXEEXT) : ipl.nas
		$(NASK) ipl.nas ipl.img
		$(FDSTAT) ipl.img

