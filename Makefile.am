dist-hook: ChangeLog

changeLog: configure.ac
	@echo "Non-maintainer upload" > ChangeLog
	@echo "" >> ChangeLog
	git log --stat --name-only --date=short --abbrev-commit >> ChangeLog

md5:
	@for f in `find ./projects -name \*.img | sort | grep -v wine`; do  \
	   echo "md5sum $${f%.*}.img > $${f%.*}.hash";                      \
	   md5sum $${f%.*}.img > $${f%.*}.hash;                             \
	   md5sum -c $${f%.*}.hash;                                         \
	done



SUBDIRS = parasol intel2gas fatlib

if BUILD_DOSFSTOOLS
   SUBDIRS += dosfstools
endif

SUBDIRS += src data projects

# functional test
ASMS  = `seq -f "%02g_nasfile.nas" 1 7`
TESTS = nask_functional_test.sh

if USE_CPPUTEST
   SUBDIRS += test
endif
