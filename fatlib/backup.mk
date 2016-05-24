#gmake

NAME = fatlib
VERSION = 0.14
PKG=$(NAME)-$(VERSION)
#CC = gcc
CFLAGS = -O -g -Wunused -Wimplicit-function-declaration
#CFLAGS = -O2 -DNDEBUG
LIB = libfat

SRC = drive.c img.c buf.c fat.c file.c dir.c del.c stdio.c \
	readdir.c seekdir.c opensubdir.c mkdir.c rmdir.c \
	delete.c dstat.c nocp.c cp.c cp850.c cp852.c
OBJ := $(filter %.o,$(SRC:.c=.o))
FILES = $(SRC) fatlib.h lib.h list.h uni_up.inc Makefile $(LIB).sym README TODO
FILES += $(addprefix examples/, Makefile ddi.c fdir.c fget.c fput.c fdstat.c)
WORK = fix

all: lib

lib: $(LIB).a

$(LIB).a: $(addprefix $(WORK)/,$(OBJ))
	ar rs $@ $^

$(WORK)/$(LIB).sym.sorted: $(LIB).sym
	@install -d $(WORK)
	@sort $^ >$@

$(WORK)/$(LIB).sym.fix: $(OBJ) $(WORK)/$(LIB).sym.sorted
	@echo Generating $@
	@nm -g $(OBJ) | sed '/^ *U/d; /:/d; /^$$/d; s/^[0-9a-f]* . //' | sort | uniq | comm -23 - $(WORK)/$(LIB).sym.sorted >$@.tmp
	@cmp -s $@.tmp $@ || mv -f $@.tmp $@

$(WORK)/$(LIB).sym.cmd: $(WORK)/$(LIB).sym.fix
	@echo Generating $@
	@sed 'h; s/^/--redefine-sym /; G; s/\n/=fat__/' $(WORK)/$(LIB).sym.fix >$@

$(WORK)/%.o: %.o $(WORK)/$(LIB).sym.cmd
	@echo Fixing symbols in $<
	@objcopy `cat $(WORK)/$(LIB).sym.cmd` $< $@
#	@objcopy -g `cat $(WORK)/$(LIB).sym.cmd` $< $@

examples:
	$(MAKE) -C examples

clean:
	rm -f $(OBJ)
	rm -f $(addprefix $(WORK)/,$(OBJ) $(LIB).sym.fix $(LIB).sym.sorted \
		$(LIB).sym.cmd $(LIB).sym.fix.tmp)
	[ -d $(WORK) ] && rmdir $(WORK); true
	$(MAKE) -C examples/ clean

install: $(LIB).a
	install -c -m 644 $(LIB).a /usr/local/lib
	install -c fat.h /usr/local/include

dist:
	ln -s . $(PKG)
	if tar --version | grep -q bsdtar; then arg='--gname root --uname root'; else arg='--group=root --owner=root'; fi; \
	tar czf $(PKG).tar.gz $$arg $(addprefix $(PKG)/, $(FILES)); \
	rm $(PKG)

.PHONY: examples clean install dist lib
