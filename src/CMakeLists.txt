message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(opennask CXX)
set(opennask_VERSION \"1.0.0\")

########### next target ###############

set(makefont_SRCS
  makefont.c)

add_executable(makefont ${makefont_SRCS})

########### next target ###############

set(bin2obj_SRCS
  bin2obj.c)

add_executable(bin2obj ${bin2obj_SRCS})

########### next target ###############

set(nask_parse_SRCS
  nask_parse.cpp
  driver.cc
  driver.hh
  demangle.hpp
  demangle.cpp
  bnfc/absyn.cc
  bnfc/absyn.hh
  bnfc/buffer.cc
  bnfc/buffer.hh
  bnfc/lexer.cc
  bnfc/parser.cc
  bnfc/parser.hh
  bnfc/printer.cc
  bnfc/printer.hh
  bnfc/parsererror.hh
  bnfc/skeleton.cc
  bnfc/skeleton.hh)

if (WIN32)
  LIST(APPEND nask_parse_SRCS getopt.h getopt.c)
  add_definitions(-Dstrcasecmp=_stricmp)
endif()

add_executable(nask_parse ${nask_parse_SRCS})
target_link_libraries(nask_parse Threads::Threads)
target_include_directories(nask_parse
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/bnfc)
set_property(TARGET nask_parse
  PROPERTY CXX_STANDARD 14)

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
