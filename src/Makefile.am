# main program
bin_PROGRAMS	  = opennask
opennask_SOURCES  = main.cpp         \
		    nask_utility.cpp \
		    nask_utility.hpp \
		    nask_defs.hpp    \
		    clogger.hpp

opennask_LDFLAGS  = -L$(top_builddir)/parasol -lparasol
opennask_CXXFLAGS = -I$(top_builddir)/parasol

# functional test
ASMS  = `seq -f "%02g_nasfile.nas" 1 7`
TESTS = nask_functional_test.sh
