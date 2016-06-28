#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([dosfstools], [2.11], [almesber@nessie.cs.id.ethz.ch])
AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([version.h])
AC_CONFIG_HEADERS([config.h])
AM_SILENT_RULES([yes])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_EXEEXT
AC_LIBTOOL_WIN32_DLL
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h limits.h mntent.h stdlib.h string.h sys/ioctl.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_OFF_T
AC_CHECK_MEMBERS([struct stat.st_rdev])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_FUNC_GETMNTENT
AC_FUNC_MALLOC
AC_CHECK_FUNCS([alarm getmntent memmove memset strchr strerror strrchr strspn strtol strtoul strtoull])

AC_CONFIG_FILES([dosfsck/Makefile
		 mkdosfs/Makefile
		 Makefile
		])
AC_OUTPUT
