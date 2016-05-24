CFLAGS = -Os -g -DDDI
LIBDIR = ..
LIB = $(LIBDIR)/libfat.a
LDFLAGS = -L.. -lfat

all: fdstat fdir fget fput

fdstat: fdstat.o ddi.o $(LIB)
	gcc fdstat.o ddi.o $(LDFLAGS) -o $@

fdir: fdir.o ddi.o $(LIB)
	gcc fdir.o ddi.o $(LDFLAGS) -o $@

fget: fget.o ddi.o $(LIB)
	gcc fget.o ddi.o $(LDFLAGS) -o $@

fput: fput.o $(LIB)
	gcc fput.o $(LDFLAGS) -o $@

clean:
	rm -f ddi.o fdstat.o fdir.o fget.o fput.o
	rm -f fdstat fdir fget fput core
