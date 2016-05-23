#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([intel2gas], [1.3.3], [mikko.tiihonen@hut.fi])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_SRCDIR([loadable.h])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
PKG_PROG_PKG_CONFIG
AC_LIBTOOL_WIN32_DLL
LT_INIT

# Compiler vendor check
AX_COMPILER_VENDOR
# Use C++11
AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
# Some compile flags
AX_CHECK_COMPILE_FLAG([-Wno-parentheses],[CXXFLAGS="$CXXFLAGS -Wno-parentheses"])

# Checks for HostOS
case "$host" in
        *-*-mingw*|*-*-cygwin*|*-*-msys*)
                ;;
        *-*-linux*)
                ;;
        *-*-darwin*)
		if test "${ax_cv_c_compiler_vendor}x" = "clangx"; then
		   CXXFLAGS="$CXXFLAGS -stdlib=libc++"
		fi
                ;;
        *-*-*bsd*)
                ;;
        *)
                AC_MSG_WARN([*** Please add $host to configure.ac checks!])
                ;;
esac

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE

# Checks for library functions.
AC_CHECK_FUNCS([memset strcasecmp strchr strdup strncasecmp strrchr strstr strtol])

# Move from "i2gconf.h.in"
#define RESOURCE_PATH "@prefix@/share/intel2gas/"
#define USE_GLOB      1
AC_DEFINE(USE_GLOB, 1, [Use Glob])
AS_IF([test "$prefix" = "NONE"],
      [AC_DEFINE_UNQUOTED([RESOURCE_PATH], ["/usr/local/share/intel2gas/"], [Define research path with --prefix dir macro])],
      [AC_DEFINE_UNQUOTED([RESOURCE_PATH], ["$prefix/share/intel2gas/"],    [Define research path with --prefix dir macro])]
)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
