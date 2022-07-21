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
set(QEMUOPT "-m" "32" "-rtc" "base=localtime" "-vga" "std" "-fda" CACHE INTERNAL "")
set(QEMU_DEBUG_OPT "-redir" "tcp:5555:127.0.0.1:1234" "&" CACHE INTERNAL "")
set(BINOPT "-fleading-underscore" "-m32" "-nostdlib" "-fno-pie" "-Wl,--oformat=binary" CACHE INTERNAL "")

#-----------------------------------------------------------------------
# Fetch matchit (c++17 pattern matching library)
#
include(FetchContent)
FetchContent_Declare(
  matchit
  GIT_REPOSITORY https://github.com/BowenFu/matchit.cpp.git
  GIT_TAG main)

FetchContent_GetProperties(matchit)
if(NOT matchit_POPULATED)
  FetchContent_Populate(matchit)
  add_subdirectory(${matchit_SOURCE_DIR} ${matchit_BINARY_DIR}
    EXCLUDE_FROM_ALL)
endif()
message(STATUS "Matchit header are present at ${matchit_SOURCE_DIR}")


add_subdirectory(parasol)
add_subdirectory(objconv)
add_subdirectory(src)
add_subdirectory(golibc)
add_subdirectory(projects)

#-----------------------------------------------------------------------
# Checks for CppUTest
#
OPTION(USE_CPPUTEST "Enable feature to test with cpputest (Dev)" ON)
if (USE_CPPUTEST)
  add_definitions(-DUSE_CPPUTEST)
  add_subdirectory(backward-cpp)
  add_subdirectory(test)
  add_subdirectory(data)
endif(USE_CPPUTEST)
