# Makefile.in for parasol
# Created by Enomoto Sanshiro on 7 July 1999.
# Last updated by Enomoto Sanshiro on 8 September 1999.
# Modified by Hiroyuki Nagata on 11 May 2016

AUTOMAKE_OPTIONS 	= foreign subdir-objects

lib_LTLIBRARIES		= libparasol.la
libparasol_la_LDFLAGS   = -no-undefined -version-info 0:0:0
libparasol_la_SOURCES	= ParaDefs.cc             \
                          ParaTokenTable.cc	  \
                          ParaToken.cc		  \
                          ParaTokenizer.cc	  \
                          ParaValue.cc		  \
                          ParaObject.cc		  \
                          ParaOperator.cc	  \
                          ParaExpression.cc	  \
                          ParaSymbolTable.cc	  \
                          ParaStatement.cc	  \
                          ParaFunction.cc	  \
                          ParaBuiltinFunction.cc  \
                          ParaPackage.cc	  \
                          ParaParser.cc		  \
                          ParaStandardLibrary.cc  \
                          ParaContainerLibrary.cc \
                          ParaMathLibrary.cc
# samples
bin_PROGRAMS = samples/statementsample samples/expressionsample samples/tokenizersample

#yaci_LIBS		= $(top_builddir)/parasol/libparasol.a
#solve_LIBS		= $(top_builddir)/parasol/libparasol.a

samples_statementsample_SOURCES  = samples/statementsample.cc
samples_statementsample_LDADD    = $(top_builddir)/libparasol.a

samples_expressionsample_SOURCES = samples/expressionsample.cc
samples_expressionsample_LDADD   = $(top_builddir)/libparasol.a

samples_tokenizersample_SOURCES  = samples/tokenizersample.cc
samples_tokenizersample_LDADD    = $(top_builddir)/libparasol.a
