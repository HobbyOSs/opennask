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
set(BINOPT "-fleading-underscore" "-m32" "-nostdlib" "-Wl,--oformat=binary" CACHE INTERNAL "")

add_subdirectory(parasol)
add_subdirectory(src)
add_subdirectory(projects)

#-----------------------------------------------------------------------
# Checks for fatlib
#
OPTION(USE_FATLIB "Enable feature fatlib (Dev)" OFF)
if (USE_FATLIB)
  add_definitions(-DUSE_FATLIB)
  add_subdirectory(fatlib)
endif(USE_FATLIB)

#-----------------------------------------------------------------------
# Checks for CppUTest
#
OPTION(USE_CPPUTEST "Enable feature to test with cpputest (Dev)" ON)
if (USE_CPPUTEST)
  add_definitions(-DUSE_CPPUTEST)
  add_subdirectory(test)
  add_subdirectory(data)
endif(USE_CPPUTEST)
