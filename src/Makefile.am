# main program
bin_PROGRAMS	  = opennask
opennask_SOURCES  = main.cpp         \
		    nask_utility.cpp \
		    nask_utility.hpp \
		    nask_defs.hpp    \
		    clogger.hpp

opennask_LDFLAGS  = -L$(top_builddir)/parasol -lparasol -lstdc++
opennask_CXXFLAGS = -I$(top_builddir)/parasol
