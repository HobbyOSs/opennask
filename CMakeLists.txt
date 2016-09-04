message(STATUS "Entering directory .")
cmake_minimum_required (VERSION 2.8)
project(opennask CXX)
set(opennask_VERSION 0.0.1)

set(PACKAGE ON CACHE STRING "opennask" FORCE)
set(PACKAGE_NAME ON CACHE STRING "opennask" FORCE)
set(PACKAGE_STRING ON CACHE STRING "opennask 0.0.1" FORCE)
set(PACKAGE_TARNAME ON CACHE STRING "opennask" FORCE)
set(PACKAGE_VERSION ON CACHE STRING "0.0.1" FORCE)
set(PACKAGE_BUGREPORT ON CACHE STRING "idiotpanzer@gmail.com" FORCE)

set(USE_CPPUTEST ON CACHE BOOL "false" FORCE)

#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# 
# AC_PREREQ([2.69])
# AC_CANONICAL_TARGET
# AM_INIT_AUTOMAKE
# AC_CONFIG_MACRO_DIR([m4])
# AC_CONFIG_SRCDIR([src/nask_defs.hpp])
# AC_CONFIG_HEADERS([config.h])
# AC_SYS_LARGEFILE
# AM_SILENT_RULES([yes])
# 
# Checks for programs.
# AC_PROG_CXXCPP
# AC_PROG_CXX_C_O
# AC_EXEEXT
# PKG_PROG_PKG_CONFIG
# AC_LIBTOOL_WIN32_DLL
# LT_INIT
# 
# Compiler vendor check
# AX_COMPILER_VENDOR
# Use C++11
# AX_CXX_COMPILE_STDCXX_11([noext], [mandatory])
# Some compile flags
# AX_CHECK_COMPILE_FLAG([-Wno-parentheses],[CXXFLAGS="$CXXFLAGS -Wno-parentheses"])
# 
# Checks for HostOS
# AC_MSG_CHECKING([for checking what kind of OS using...])
# case "$host" in
#         *-*-mingw*|*-*-cygwin*|*-*-msys*)
# 		AC_MSG_RESULT([DOS, define BUILD_DOSFSTOOLS as true])
# 		AM_CONDITIONAL(BUILD_DOSFSTOOLS, true)
# 		AC_DEFINE(BUILD_DOSFSTOOLS, 1, [build dosfstools])
#                 ;;
#         *-*-linux*)
# 		AC_MSG_RESULT([Linux, define BUILD_DOSFSTOOLS as true])
# 		AM_CONDITIONAL(BUILD_DOSFSTOOLS, true)
# 		AC_DEFINE(BUILD_DOSFSTOOLS, 1, [build dosfstools])
#                 ;;
#         *-*-darwin*)
# 		if test "${ax_cv_c_compiler_vendor}x" = "clangx"; then
# 		   CXXFLAGS="$CXXFLAGS -stdlib=libc++"
# 		fi
# 		AC_MSG_RESULT([Mac, define BUILD_DOSFSTOOLS as false])
# 		AM_CONDITIONAL(BUILD_DOSFSTOOLS, false)
#                 ;;
#         *-*-*bsd*)
# 		AC_MSG_RESULT([BSD, define BUILD_DOSFSTOOLS as true])
# 		AM_CONDITIONAL(BUILD_DOSFSTOOLS, true)
# 		AC_DEFINE(BUILD_DOSFSTOOLS, 1, [build dosfstools])
#                 ;;
#         *)
#                 AC_MSG_WARN([*** Please add $host to configure.ac checks!])
#                 ;;
# esac
# 
#-----------------------------------------------------------------------
# Checks for CppUTest
#
# AC_ARG_ENABLE([cpputest],
#     AS_HELP_STRING([--enable-cpputest], [Enable feature to test with cpputest (Dev)]))
# 
# AS_IF([test "x$enable_cpputest" = "xyes"], [
#    PKG_CHECK_MODULES([CPPUTEST], [cpputest], [has_cpputest=1], [has_cpputest=0])
#    AC_MSG_CHECKING([for whether to support cpputest])
#    if test $has_cpputest = 1; then
#       AM_CONDITIONAL(USE_CPPUTEST, true)
#       AC_DEFINE(USE_CPPUTEST, 1, [when building unit test code])
#       AC_MSG_RESULT(yes)
#    else
#       AC_MSG_RESULT(no)
#    fi
# ])
# 
# Checks for header files.
# 
# Checks for typedefs, structures, and compiler characteristics.
# AC_CHECK_HEADER_STDBOOL
# AC_C_INLINE
# 
# Checks for library functions.
# 
# AC_CONFIG_FILES([Makefile
# 		 src/Makefile
# 		 data/Makefile
# 		 projects/Makefile
# 		 projects/01_day/Makefile
# 		 projects/02_day/Makefile
# 		 projects/03_day/Makefile
# 		 projects/01_day/helloos1/Makefile
# 		 projects/01_day/helloos2/Makefile
# 		 projects/02_day/helloos3/Makefile
# 		 projects/02_day/helloos4/Makefile
# 		 projects/02_day/helloos5/Makefile
# 		 projects/03_day/harib00a/Makefile
# 		 projects/03_day/harib00b/Makefile
# 		 projects/03_day/harib00c/Makefile
# 		 projects/03_day/harib00d/Makefile
# 		 projects/03_day/harib00e/Makefile
# 		 projects/03_day/harib00f/Makefile
# 		 projects/03_day/harib00g/Makefile
# 		 projects/03_day/harib00h/Makefile
# 		 projects/03_day/harib00i/Makefile
# 		 projects/03_day/harib00j/Makefile
# 		 ])
# 
# AC_CONFIG_SUBDIRS([parasol intel2gas fatlib])
# AM_COND_IF([BUILD_DOSFSTOOLS], [AC_CONFIG_SUBDIRS([dosfstools])])
# AM_COND_IF([USE_CPPUTEST], AC_CONFIG_FILES([test/Makefile]))
# 
# AC_OUTPUT
# 
# AM_COND_IF([BUILD_DOSFSTOOLS],
# 	   [AC_MSG_RESULT([dosfstools  :	  build])],
# 	   [AC_MSG_RESULT([dosfstools  :	  none])])
# AM_COND_IF([USE_CPPUTEST],
# 	   [AC_MSG_RESULT([cpputest  :		  build])],
# 	   [AC_MSG_RESULT([cpputest  :		  none])])
add_subdirectory(parasol)
add_subdirectory(intel2gas)
add_subdirectory(fatlib)

