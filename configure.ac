#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([opennask], [0.0.1], [idiotpanzer@gmail.com])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AC_SYS_LARGEFILE

# Checks for programs.
AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
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

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
		 data/Makefile])
AC_CONFIG_SUBDIRS([parasol])
AC_OUTPUT
