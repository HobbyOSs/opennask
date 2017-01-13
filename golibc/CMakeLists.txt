message(STATUS "Entering directory golibc/")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

set(go_STAT_SRCS
    strcat.c
    strlen.c
    memmove.c
    ldexp.c
    strchr.c
    qsort.c
    strcmp.c
    #strtol.c
    strncmp.c
    memchr.c
    strcpy.c
    strncat.c
    strstr.c
    #strtoul.c
    atof.c
    strdup.c
    strncpy.c
    memcmp.c
    abs.c
    memcpy.c
    memset.c
    errno.c
    strspn.c
    strpbrk.c
    atoi.c
    strrchr.c
    strcspn.c
    frexp.c
    strtoul0.c)

if (NOT WIN32 AND NOT APPLE)
  LIST(APPEND go_STAT_SRCS sprintf.c vsprintf.c)
endif()

set(go_HEADERS
    stdarg.h
    string.h
    stddef.h
    math.h
    stdio.h
    setjmp.h
    float.h
    errno.h
    stdlib.h
    limits.h)

set(GCC_COVERAGE_COMPILE_FLAGS "-m32" "-nostdlib")
add_definitions(${GCC_COVERAGE_COMPILE_FLAGS})

add_library(go STATIC ${go_STAT_SRCS})
