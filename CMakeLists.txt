message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
enable_testing()
project(root)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
set(USE_CPPUTEST ON CACHE BOOL "false" FORCE)

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
OPTION(USE_CPPUTEST "Enable feature to test with cpputest (Dev)" OFF)
if (DEFINED USE_CPPUTEST)
  add_definitions( -DUSE_CPPUTEST )
else()

endif()

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
# AM_COND_IF([BUILD_DOSFSTOOLS], [AC_CONFIG_SUBDIRS([dosfstools])])
# AM_COND_IF([USE_CPPUTEST], AC_CONFIG_FILES([test/Makefile]))
#
add_subdirectory(parasol)
add_subdirectory(intel2gas)
add_subdirectory(fatlib)
add_subdirectory(src)
add_subdirectory(test)
