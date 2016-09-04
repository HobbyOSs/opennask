message(STATUS "Entering directory fatlib/")

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


########### next target ###############

set(examples/fdstat_SRCS ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)


########### next target ###############

set(examples/fdir_SRCS ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)


########### next target ###############

set(examples/fget_SRCS ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)


########### next target ###############

set(examples/fput_SRCS ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)

