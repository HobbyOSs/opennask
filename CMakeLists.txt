message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
enable_testing()
project(root CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
project(root)

set(WINE "/usr/bin/wine" CACHE INTERNAL "")
set(WINE_NASK "~/.wine/drive_c/MinGW/msys/1.0/bin/nask.exe" CACHE INTERNAL "")
set(OD "od" CACHE INTERNAL "")
set(QEMU "qemu-system-i386" CACHE INTERNAL "")
set(QEMUOPT "-m" "32" "-localtime" "-vga" "std" "-fda" CACHE INTERNAL "")
set(ENV{I2G_DATA} "${root_SOURCE_DIR}/intel2gas/")
set(ENV{G2I_DATA} "${root_SOURCE_DIR}/intel2gas/")

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
#
# AM_COND_IF([BUILD_DOSFSTOOLS], [AC_CONFIG_SUBDIRS([dosfstools])])
# AM_COND_IF([USE_CPPUTEST], AC_CONFIG_FILES([test/Makefile]))
#
# AM_COND_IF([BUILD_DOSFSTOOLS], [AC_CONFIG_SUBDIRS([dosfstools])])
# AM_COND_IF([USE_CPPUTEST], AC_CONFIG_FILES([test/Makefile]))
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
add_subdirectory(src)

#-----------------------------------------------------------------------
# Checks for CppUTest
#
OPTION(USE_CPPUTEST "Enable feature to test with cpputest (Dev)" ON)
if (USE_CPPUTEST)
  add_definitions(-DUSE_CPPUTEST)
  add_subdirectory(test)
  add_subdirectory(data)
endif(USE_CPPUTEST)

add_subdirectory(projects)
