message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(opennask CXX)
set(opennask_VERSION \"0.0.3\")

########### next target ###############

set(makefont_SRCS
    makefont.c)

add_executable(makefont ${makefont_SRCS})

########### next target ###############

set(opennask_SRCS
    main.cpp
    bracket_impl.hpp
    bracket_utility.hpp
    mod_rm.cpp
    mod_rm.hpp
    string_util.cpp
    string_util.hpp
    nask_utility.cpp
    nask_utility.hpp
    nask_defs.hpp
    tinyexpr.c
    tinyexpr.h)

### versioning
add_definitions(-DOPENNASK_VERSION=${opennask_VERSION})

### thanks https://github.com/takamin/win-c ###
if (WIN32)
  LIST(APPEND opennask_SRCS getopt.h getopt.c)
  add_definitions(-Dstrcasecmp=_stricmp)
endif()



include_directories(${root_SOURCE_DIR}/parasol ${root_SOURCE_DIR}/src)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(opennask ${opennask_SRCS})
target_link_libraries(opennask parasol Threads::Threads)

set_property(TARGET opennask PROPERTY CXX_STANDARD 11)
set_property(TARGET opennask PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET opennask PROPERTY APPEND_STRING PROPERTY COMPILE_FLAGS -Wno-missing-braces)
