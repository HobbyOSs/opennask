message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(opennask CXX)
set(opennask_VERSION 0.0.1)

set(PACKAGE ON CACHE STRING "opennask" FORCE)
set(PACKAGE_NAME ON CACHE STRING "opennask" FORCE)
set(PACKAGE_STRING ON CACHE STRING "opennask 0.0.1" FORCE)
set(PACKAGE_TARNAME ON CACHE STRING "opennask" FORCE)
set(PACKAGE_VERSION ON CACHE STRING "0.0.1" FORCE)
set(PACKAGE_BUGREPORT ON CACHE STRING "idiotpanzer@gmail.com" FORCE)

########### next target ###############

set(opennask_SRCS
    main.cpp
    bracket_impl.hpp
    mod_rm.cpp
    mod_rm.hpp
    string_util.cpp
    string_util.hpp
    nask_utility.cpp
    nask_utility.hpp
    nask_defs.hpp
    tinyexpr.c
    tinyexpr.h
    coff.h)

include_directories(${root_SOURCE_DIR}/parasol
                    ${root_SOURCE_DIR}/src
                    ${root_SOURCE_DIR}/fatlib)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(opennask ${opennask_SRCS})
target_link_libraries(opennask parasol Threads::Threads)

set_property(TARGET opennask PROPERTY CXX_STANDARD 11)
set_property(TARGET opennask PROPERTY CXX_STANDARD_REQUIRED ON)
