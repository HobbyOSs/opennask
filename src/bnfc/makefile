## File generated by the BNF Converter (bnfc 2.9.5).

CC=g++ -g
CCFLAGS=-std=c++14 -W -Wall -Wsign-conversion -Wno-unused-parameter -Wno-unused-function -Wno-unneeded-internal-declaration

FLEX=flex
FLEX_OPTS=-Pnask_

BISON=bison
BISON_OPTS=-t -pnask_

OBJS=absyn.o buffer.o lexer.o parser.o driver.o printer.o

.PHONY : clean distclean

all : testnask

clean :
	rm -f *.o testnask nask.aux nask.log nask.pdf nask.dvi nask.ps nask

distclean : clean
	rm -f absyn.cc absyn.hh buffer.cc buffer.hh test.cc Bison.hh parser.cc parser.hh parserError.hh nask.yy lexer.cc nask.ll skeleton.cc skeleton.hh printer.cc printer.hh driver.cc driver.hh scanner.hh location.hh Makefile nask.tex

testnask : ${OBJS} test.o
	@echo "Linking testnask..."
	${CC} ${OBJS} test.o -o testnask

absyn.o : absyn.cc absyn.hh
	${CC} ${CCFLAGS} -c absyn.cc

driver.o : driver.cc driver.hh
	${CC} ${CCFLAGS} -c driver.cc

buffer.o : buffer.cc buffer.hh
	${CC} ${CCFLAGS} -c buffer.cc

lexer.cc : nask.ll
	${FLEX} ${FLEX_OPTS} -olexer.cc nask.ll

parser.cc Bison.hh : nask.yy
	${BISON} ${BISON_OPTS} nask.yy -o parser.cc

lexer.o : CCFLAGS+=-Wno-sign-conversion

lexer.o : lexer.cc Bison.hh
	${CC} ${CCFLAGS} -c lexer.cc

parser.o : parser.cc absyn.hh Bison.hh
	${CC} ${CCFLAGS} -c parser.cc

printer.o : printer.cc printer.hh absyn.hh
	${CC} ${CCFLAGS} -c printer.cc

skeleton.o : skeleton.cc skeleton.hh absyn.hh
	${CC} ${CCFLAGS} -Wno-unused-parameter -c skeleton.cc

test.o : test.cc parser.hh printer.hh absyn.hh
	${CC} ${CCFLAGS} -c test.cc
