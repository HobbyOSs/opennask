#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([libfat], [0.1.4], [jb@wizard·ae·krakow·pl])
AC_CONFIG_SRCDIR([lib.h])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([foreign -Wno-portability])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lfat':
# AC_CHECK_LIB([fat], [main])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdint.h stdlib.h string.h sys/file.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_OFF_T
AC_TYPE_SIZE_T
AC_LIBTOOL_WIN32_DLL
LT_INIT

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([atexit localtime_r memset strchr])

AC_CONFIG_FILES([Makefile])
dnl              examples/Makefile])
AC_OUTPUT
