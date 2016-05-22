
dnl --------------------------------
dnl Initialization macros.
dnl --------------------------------

AC_INIT([reflcpp], [0.2], [bug-report@address])
dnl AC_INIT(reflcpp/Serializer.h)
AM_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR([m4])

dnl -----------------------------------------------
dnl Package name and version number (user defined)
dnl -----------------------------------------------

GENERIC_LIBRARY_NAME=reflcpp

#release versioning
GENERIC_MAJOR_VERSION=0
GENERIC_MINOR_VERSION=2
GENERIC_MICRO_VERSION=0

#API version (often = GENERIC_MAJOR_VERSION.GENERIC_MINOR_VERSION)
GENERIC_API_VERSION=0.2
AC_SUBST(GENERIC_API_VERSION)

#shared library versioning
GENERIC_LIBRARY_VERSION=0:2:0
#                       | | |
#                +------+ | +---+
#                |        |     |
#             current:revision:age
#                |        |     |
#                |        |     +- increment if interfaces have been added
#                |        |        set to zero if interfaces have been removed
#                                  or changed
#                |        +- increment if source code has changed
#                |           set to zero if current is incremented
#                +- increment if interfaces have been added, removed or changed


dnl --------------------------------
dnl Package name and version number
dnl --------------------------------

AC_SUBST(GENERIC_LIBRARY_VERSION)

PACKAGE=$GENERIC_LIBRARY_NAME
AC_SUBST(GENERIC_LIBRARY_NAME)

GENERIC_VERSION=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION.$GENERIC_MICRO_VERSION
GENERIC_RELEASE=$GENERIC_MAJOR_VERSION.$GENERIC_MINOR_VERSION
AC_SUBST(GENERIC_RELEASE)
AC_SUBST(GENERIC_VERSION)

VERSION=$GENERIC_VERSION

dnl Directory where the library will get installed
LIBRARY_DIR=$GENERIC_LIBRARY_NAME-$GENERIC_API_VERSION/$GENERIC_LIBRARY_NAME
AC_SUBST(LIBRARY_DIR)

dnl AM_INIT_AUTOMAKE($PACKAGE, $VERSION, no-define)
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

dnl -----------------------------------------------
dnl Checks for programs.
dnl -----------------------------------------------

AC_PROG_CC
AC_PROG_CXX
AM_PROG_AR
AM_PROG_LIBTOOL
AM_SANITY_CHECK

AC_LANG_CPLUSPLUS

dnl -----------------------------------------------
dnl Checks for libraries.
dnl -----------------------------------------------

PKG_CHECK_MODULES([LIBXML2], [libxml-2.0 >= 2.0.0], [HAVE_LIBXML2=1], [HAVE_LIBXML2=0])
AM_CONDITIONAL([USE_LIBXML2], [test "$HAVE_LIBXML2" -eq 1])

dnl -----------------------------------------------
dnl Generates Makefile's, configuration files and scripts
dnl -----------------------------------------------

dnl test/Makefile <-- I can't compile it :<
AM_COND_IF([USE_LIBXML2],
	[AC_SUBST(LIBXML2_CFLAGS)
	 AC_SUBST(LIBXML2_LIBS)
	 AC_CONFIG_FILES([tools/codegenerator/Makefile])])

AC_OUTPUT([
	Makefile
	reflcpp.pc
	reflcpp/Makefile
	tools/Makefile
])
