#
# Generated 2016年  6月  7日 火曜日 22:36:25 JST 
#

NASK = $(top_builddir)/src/opennask$(EXEEXT)
FDSTAT = $(top_builddir)/fatlib/examples/fdstat$(EXEEXT)

bin_PROGRAMS =  ipl.img
ipl_img_SOURCES = ipl.nas

ipl.img$(EXEEXT) : ipl.nas
		$(NASK) ipl.nas ipl.img
		$(FDSTAT) ipl.img

