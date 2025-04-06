message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")
project(root)

#-----------------------------------------------------------------------
# Dev macro (uncommend if you want to use)
#
#set(CMAKE_BUILD_TYPE "Debug" CACHE INTERNAL "cmake build type")
#set(CMAKE_C_COMPILER "/usr/bin/clang" CACHE INTERNAL "clang compiler")
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++" CACHE INTERNAL "clang++ compiler")

#-----------------------------------------------------------------------
# tools
#
# Find gosk assembler
find_program(GOSK_EXECUTABLE gosk)
if(NOT GOSK_EXECUTABLE)
    message(FATAL_ERROR "gosk not found. Please install gosk and ensure it is in your PATH.")
endif()
message(STATUS "Found gosk: ${GOSK_EXECUTABLE}")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-inconsistent-missing-override")
set(WINE "/usr/bin/wine" CACHE INTERNAL "")
set(WINE_NASK "~/.wine/drive_c/MinGW/msys/1.0/bin/nask.exe" CACHE INTERNAL "") # Keep for wine debug
set(OD "od" CACHE INTERNAL "")
set(QEMU "qemu-system-i386" CACHE INTERNAL "")
set(QEMUOPT "-m" "32" "-rtc" "base=localtime" "-vga" "std" "-fda" CACHE INTERNAL "")
set(QEMU_DEBUG_OPT "-redir" "tcp:5555:127.0.0.1:1234" "&" CACHE INTERNAL "")
set(BINOPT "-fleading-underscore" "-m32" "-nostdlib" "-fno-pie" "-Wl,--oformat=binary" CACHE INTERNAL "")

add_subdirectory(golibc)
add_subdirectory(projects)
