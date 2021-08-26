CC=g++ -g
CCFLAGS=--ansi -W -Wall -Wno-unused-parameter -Wno-unused-function -Wno-unneeded-internal-declaration

FLEX=flex
FLEX_OPTS=-Pnask_

BISON=bison
BISON_OPTS=-t -pnask_

OBJS=Absyn.o Buffer.o Lexer.o Parser.o Printer.o

.PHONY : clean distclean

all : TestNask

clean :
	rm -f *.o TestNask Nask.aux Nask.log Nask.pdf Nask.dvi Nask.ps Nask

distclean : clean
	rm -f Absyn.C Absyn.H Buffer.C Buffer.H Test.C Bison.H Parser.C Parser.H ParserError.H Nask.y Lexer.C Nask.l Skeleton.C Skeleton.H Printer.C Printer.H Makefile Nask.tex

TestNask : ${OBJS} Test.o
	@echo "Linking TestNask..."
	${CC} ${OBJS} Test.o -o TestNask

Absyn.o : Absyn.C Absyn.H
	${CC} ${CCFLAGS} -c Absyn.C

Buffer.o : Buffer.C Buffer.H
	${CC} ${CCFLAGS} -c Buffer.C

Lexer.C : Nask.l
	${FLEX} ${FLEX_OPTS} -oLexer.C Nask.l

Parser.C Bison.H : Nask.y
	${BISON} ${BISON_OPTS} Nask.y -o Parser.C

Lexer.o : Lexer.C Bison.H
	${CC} ${CCFLAGS} -c Lexer.C

Parser.o : Parser.C Absyn.H Bison.H
	${CC} ${CCFLAGS} -c Parser.C

Printer.o : Printer.C Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Printer.C

Skeleton.o : Skeleton.C Skeleton.H Absyn.H
	${CC} ${CCFLAGS} -Wno-unused-parameter -c Skeleton.C

Test.o : Test.C Parser.H Printer.H Absyn.H
	${CC} ${CCFLAGS} -c Test.C
