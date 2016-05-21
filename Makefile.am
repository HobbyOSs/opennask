dist-hook: ChangeLog

changeLog: configure.ac
	@echo "Non-maintainer upload" > ChangeLog
	@echo "" >> ChangeLog
	git log --stat --name-only --date=short --abbrev-commit >> ChangeLog

SUBDIRS = parasol data

# main program
bin_PROGRAMS	  = opennask
opennask_SOURCES  = main.cpp nask_utility.cpp
opennask_LDFLAGS  = -L./parasol -lparasol
opennask_CXXFLAGS = -I$(top_builddir)/parasol

# functional test
ASMS  = `seq -f "%02g_nasfile.nas" 1 7`
TESTS = nask_functional_test.sh

all-local: compile-all-asms
	make -C data md5

compile-all-asms:
	@ASMS="$(ASMS)";                                        \
	for x in $$ASMS; do                                     \
		echo "== compile $$x ...";                      \
		./opennask data/$x data/`basename $x .nas`.img; \
	done

show-all-asms:
	@ASMS="$(ASMS)";       \
	for x in $$ASMS; do    \
		echo "== $$x"; \
	done
