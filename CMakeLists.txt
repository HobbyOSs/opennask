message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(root CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
project(root)

#set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE INTERNAL "clang compiler")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE INTERNAL "clang++ compiler")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")

set(WINE "/usr/bin/wine" CACHE INTERNAL "")
set(WINE_NASK "~/.wine/drive_c/MinGW/msys/1.0/bin/nask.exe" CACHE INTERNAL "")
set(OD "od" CACHE INTERNAL "")
set(QEMU "qemu-system-i386" CACHE INTERNAL "")
set(QEMUOPT "-m" "32" "-rtc" "base=localtime" "-vga" "std" "-fda" CACHE INTERNAL "")
set(QEMU_DEBUG_OPT "-redir" "tcp:5555:127.0.0.1:1234" "&" CACHE INTERNAL "")
set(BINOPT "-fleading-underscore" "-m32" "-nostdlib" "-fno-pie" "-Wl,--oformat=binary" CACHE INTERNAL "")

add_subdirectory(matchit.cpp)
add_subdirectory(parasol)
add_subdirectory(objconv)
add_subdirectory(spdlog)
add_subdirectory(src)
add_subdirectory(golibc)
add_subdirectory(projects)

#-----------------------------------------------------------------------
# Test
#
OPTION(USE_GTEST "Enable feature to test with googletest (Dev)" ON)
OPTION(WITH_BACKWARDS_CPP "Build with backwards-cpp stack trace dumping library? (Linux-only)" OFF)
message(STATUS "Use backward-cpp for stack traces? " ${WITH_BACKWARDS_CPP})

if(WITH_BACKWARDS_CPP)
  # Support 'backward-cpp,' a lean stacktrace printing library for Linux.
  add_definitions(-DWITH_BACKWARDS_CPP)
  add_subdirectory(backward-cpp)
endif()

if (USE_GTEST)
  enable_testing()
  add_subdirectory(googletest)
  add_subdirectory(test)
  add_subdirectory(data)
endif(USE_GTEST)
