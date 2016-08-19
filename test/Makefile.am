AUTOMAKE_OPTIONS = subdir-objects
TESTS = alltest

bin_PROGRAMS    = alltest
alltest_SOURCES = alltest.cpp        \
                  example_suite.cpp  \
                  tinyexpr_suite.cpp \
                  ../src/tinyexpr.c

alltest_CXXFLAGS = -I$(top_builddir)/src
alltest_LDADD    = -L/usr/lib -lCppUTest -lCppUTestExt
