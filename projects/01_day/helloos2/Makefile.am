#
# Generated 2016年  5月 25日 水曜日 14:16:34 JST 
#

NASK = $(top_builddir)/src/opennask$(EXEEXT)
FDSTAT = $(top_builddir)/fatlib/examples/fdstat$(EXEEXT)

bin_PROGRAMS =  helloos.img
helloos_img_SOURCES = helloos.nas

helloos.img$(EXEEXT) : helloos.nas
		$(NASK) helloos.nas helloos.img
		$(FDSTAT) helloos.img

