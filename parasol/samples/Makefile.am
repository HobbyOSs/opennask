# Makefile.in for parasol sample programs
# Created by Enomoto Sanshiro on 7 July 1999.
# Last updated by Enomoto Sanshiro on 8 September 1999.

bin_PROGRAMS = yaci solve statementsample expressionsample tokenizersample

yaci_SOURCES		=
yaci_LDADD		= $(top_builddir)/parasol/libparasol.a
solve_LDADD		= $(top_builddir)/parasol/libparasol.a
statementsample_LDADD	= $(top_builddir)/parasol/libparasol.a
expressionsample_LDADD	= $(top_builddir)/parasol/libparasol.a
tokenizersample_LDADD	= $(top_builddir)/parasol/libparasol.a

# OBJS =  AdditionalOperators.o AdditionalStatements.o

yaci: yaci.o ../libparasol.a
	$(CXX) $(CXXFLAGS) -o $@ $@.o -L.. -lparasol $(LIBS)


solve: solve.o ../libparasol.a
	$(CXX) $(CXXFLAGS) -o $@ $@.o -L.. -lparasol $(LIBS)


statementsample: statementsample.o ../libparasol.a
	$(CXX) $(CXXFLAGS) -o $@ $@.o -L.. -lparasol $(LIBS)


expressionsample: expressionsample.o ../libparasol.a
	$(CXX) $(CXXFLAGS) -o $@ $@.o -L.. -lparasol $(LIBS)


tokenizersample: tokenizersample.o ../libparasol.a
	$(CXX) $(CXXFLAGS) -o $@ $@.o -L.. -lparasol $(LIBS)


.cc.o:
	$(CXX) $(CPPFLAGS) -I.. $(CXXFLAGS) $(DEFS) -c $<


clean:
	-rm -f *.o
	-rm -f $(TARGETS)
