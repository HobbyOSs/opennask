dnl configure.in for kinoko
dnl Created by Enomoto Sanshiro on 16 June 1999.
dnl Last updated by Enomoto Sanshiro on 9 November 2000.
AC_PREREQ([2.69])
AC_INIT([parasol], [0.5.2], [idiotpanzer@gmail.com])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AC_CONFIG_MACRO_DIR([m4])
AC_SYS_LARGEFILE

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

AC_PROG_AWK
AC_PROG_CXX
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_MKDIR_P
PKG_PROG_PKG_CONFIG
AC_LIBTOOL_WIN32_DLL
LT_INIT

###### General Settings ######

# Checks for libm
AC_CHECK_LIB(m,sin,LIBS="-lm $LIBS" has_m=1,has_m=0)
if test $has_m = 0; then
  AC_MSG_ERROR("ERROR: cannot find libm.")
fi

###### Output ######

AC_OUTPUT([Makefile])
