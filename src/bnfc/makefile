CC=g++ -g
CCFLAGS=--ansi -W -Wall -Wno-unused-parameter -Wno-unused-function -Wno-unneeded-internal-declaration -DYYDEBUG

FLEX=flex
FLEX_OPTS=-Pnask_

BISON=bison
BISON_OPTS=-t -pnask_

OBJS=absyn.o buffer.o lexer.o parser.o printer.o

.PHONY : clean distclean

all : testnask

clean :
	rm -f *.o testnask nask.aux nask.log nask.pdf nask.dvi nask.ps nask

distclean : clean
	rm -f absyn.cc absyn.hh buffer.cc buffer.hh test.cc bison.hh parser.cc parser.hh parserError.hh nask.y lexer.cc nask.l skeleton.cc skeleton.hh printer.cc printer.hh Makefile nask.tex

testnask : ${OBJS} test.o
	@echo "Linking testnask..."
	${CC} ${OBJS} test.o -o testnask

absyn.o : absyn.cc absyn.hh
	${CC} ${CCFLAGS} -c absyn.cc

buffer.o : buffer.cc buffer.hh
	${CC} ${CCFLAGS} -c buffer.cc

lexer.cc : nask.l
	${FLEX} ${FLEX_OPTS} -olexer.cc nask.l

parser.cc bison.hh : nask.y
	${BISON} ${BISON_OPTS} nask.y -o parser.cc

lexer.o : lexer.cc bison.hh
	${CC} ${CCFLAGS} -c lexer.cc

parser.o : parser.cc absyn.hh bison.hh
	${CC} ${CCFLAGS} -c parser.cc

printer.o : printer.cc printer.hh absyn.hh
	${CC} ${CCFLAGS} -c printer.cc

skeleton.o : skeleton.cc skeleton.hh absyn.hh
	${CC} ${CCFLAGS} -Wno-unused-parameter -c skeleton.cc

test.o : test.cc parser.hh printer.hh absyn.hh
	${CC} ${CCFLAGS} -c test.cc
