
library_includedir = $(includedir)/$(LIBRARY_DIR)
library_include_HEADERS =       \
	Arguments.hpp           \
	ArrayType.hpp           \
	BoundClassType.hpp      \
	BoundClassType_tmpl.hpp \
	BoundMemberFunction.hpp \
	ClassType.hpp           \
	ClassType_tmpl.hpp      \
	Exceptions.hpp          \
	FundamentalType.hpp     \
	MemberFunction.hpp      \
	MemberFunctionDcl.hpp   \
	PointerType.hpp         \
	PtrHolder.hpp           \
	RetValHolder.hpp        \
	Traits.hpp              \
	Type.hpp                \
	TypeInfoCmp.hpp         \
	check.hpp               \
	debug.hpp               \
	macros.hpp              \
	platform.hpp

AM_CPPFLAGS = -I$(top_srcdir)

lib_LTLIBRARIES = libreflcpp.la
libreflcpp_la_SOURCES = \
	Arguments.cpp \
	ArrayType.cpp \
	check.cpp \
	ClassType.cpp \
	FundamentalType.cpp \
	PointerType.cpp \
	Type.cpp

libreflcpp_la_LDFLAGS = -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)
