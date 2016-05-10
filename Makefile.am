dist-hook: ChangeLog

changeLog: configure.ac
	@echo "Non-maintainer upload" > ChangeLog
	@echo "" >> ChangeLog
	git log --stat --name-only --date=short --abbrev-commit >> ChangeLog

SUBDIRS = parasol

bin_PROGRAMS	  = opennask
opennask_SOURCES  = main.cpp
opennask_LDFLAGS  = -L./parasol -lparasol
opennask_CXXFLAGS = -I$(top_builddir)/parasol
