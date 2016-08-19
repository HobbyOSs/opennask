AUTOMAKE_OPTIONS = subdir-objects
TESTS = alltest

bin_PROGRAMS    = alltest
alltest_SOURCES = alltest.cpp        \
                  nask_suite.cpp     \
                  tinyexpr_suite.cpp \
                  ../src/tinyexpr.c  \
                  ../src/nask_utility.cpp

alltest_CXXFLAGS = -I$(top_builddir)/parasol -I$(top_builddir)/src
alltest_LDADD    = $(top_builddir)/parasol/libparasol.a -L/usr/lib -lCppUTest -lCppUTestExt
