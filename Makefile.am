dist-hook: ChangeLog

changeLog: configure.ac
	@echo "Non-maintainer upload" > ChangeLog
	@echo "" >> ChangeLog
	git log --stat --name-only --date=short --abbrev-commit >> ChangeLog

SUBDIRS = parasol intel2gas fatlib dosfstools src data projects

# functional test
ASMS  = `seq -f "%02g_nasfile.nas" 1 7`
TESTS = nask_functional_test.sh

compile-all-asms:
	@ASMS="$(ASMS)";                                        \
	for x in $$ASMS; do                                     \
		echo "== compile $$x ...";                      \
		./opennask data/$x data/`basename $x .nas`.img; \
	done
	make -C data md5

show-all-asms:
	@ASMS="$(ASMS)";       \
	for x in $$ASMS; do    \
		echo "== $$x"; \
	done
