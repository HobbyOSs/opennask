message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(fat C)
set(libfat_VERSION 0.1.4)

set(PACKAGE ON CACHE STRING "libfat" FORCE)
set(PACKAGE_NAME ON CACHE STRING "libfat" FORCE)
set(PACKAGE_STRING ON CACHE STRING "libfat 0.1.4" FORCE)
set(PACKAGE_TARNAME ON CACHE STRING "libfat" FORCE)
set(PACKAGE_VERSION ON CACHE STRING "0.1.4" FORCE)
set(PACKAGE_BUGREPORT ON CACHE STRING "jb@wizard·ae·krakow·pl" FORCE)

########### next target ###############

set(fat_STAT_SRCS
    drive.c
    img.c
    buf.c
    fat.c
    file.c
    dir.c
    del.c
    stdio.c
    readdir.c
    seekdir.c
    opensubdir.c
    mkdir.c
    rmdir.c
    delete.c
    dstat.c
    nocp.c
    cp.c
    cp850.c
    cp852.c)

set(fat_STAT_HEADERS
    fatlib.h
    lib.h
    list.h)

add_library(fat STATIC ${fat_STAT_SRCS} ${fat_STAT_HEADERS})

########### next target ###############

set(examples_fdstat_SRCS examples/fdstat.c examples/ddi.c)
add_executable(examples_fdstat ${examples_fdstat_SRCS})
include_directories(${PROJECT_SOURCE_DIR}/fatlib)
add_definitions( -DDDI )
target_link_libraries(examples_fdstat fat)

########### next target ###############

set(examples_fdir_SRCS examples/fdir.c examples/ddi.c)
add_executable(examples_fdir ${examples_fdir_SRCS})
include_directories(${PROJECT_SOURCE_DIR}/fatlib)
add_definitions( -DDDI )
target_link_libraries(examples_fdir fat)

########### next target ###############

set(examples_fget_SRCS examples/fget.c examples/ddi.c)
add_executable(examples_fget ${examples_fget_SRCS})
include_directories(${PROJECT_SOURCE_DIR}/fatlib)
add_definitions( -DDDI )
target_link_libraries(examples_fget fat)

########### next target ###############

set(examples_fput_SRCS examples/fput.c)
add_executable(examples_fput ${examples_fput_SRCS})
include_directories(${PROJECT_SOURCE_DIR}/fatlib)
add_definitions( -DDDI )
target_link_libraries(examples_fput fat)
