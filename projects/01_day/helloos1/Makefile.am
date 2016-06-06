#
# Generated 2016年  6月  7日 火曜日 00:00:44 JST 
#

NASK = $(top_builddir)/src/opennask$(EXEEXT)
FDSTAT = $(top_builddir)/fatlib/examples/fdstat$(EXEEXT)

bin_PROGRAMS =  helloos.img
helloos_img_SOURCES = helloos.nas

helloos.img$(EXEEXT) : helloos.nas
		$(NASK) helloos.nas helloos.img
		$(FDSTAT) helloos.img

