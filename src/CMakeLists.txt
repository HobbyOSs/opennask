message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(opennask CXX)
set(opennask_VERSION \"1.0.1\")

find_package(Threads REQUIRED)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

include_directories(
  ${root_SOURCE_DIR}/parasol
  ${root_SOURCE_DIR}/src/incbin
  ${root_SOURCE_DIR}/src
  ${root_SOURCE_DIR}/spdlog/include
  ${root_SOURCE_DIR}/jsoncons/include
  ${matchit_SOURCE_DIR}/include)

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
  front_end.cc
  front_end.hh
  pass1_strategy.cc
  pass1_strategy.hh
  para_token.cc
  para_token.hh
  demangle.hpp
  demangle.cpp
  mod_rm.cpp
  mod_rm.hpp
  string_util.cpp
  string_util.hpp
  label.cpp
  label.hpp
  bin_util.cc
  bin_util.hh
  bnfc/absyn.cc
  bnfc/absyn.hh
  bnfc/buffer.cc
  bnfc/buffer.hh
  bnfc/driver.cc
  bnfc/driver.hh
  bnfc/lexer.cc
  bnfc/parser.cc
  bnfc/parser.hh
  bnfc/printer.cc
  bnfc/printer.hh
  bnfc/parsererror.hh
  bnfc/skeleton.cc
  bnfc/skeleton.hh
  incbin/incbin.h)

if (WIN32)
  LIST(APPEND nask_parse_SRCS getopt.h getopt.c)
  add_definitions(-Dstrcasecmp=_stricmp)
endif()

add_executable(nask_parse
  ${nask_parse_SRCS})
target_link_libraries(nask_parse PRIVATE
  Threads::Threads
  spdlog::spdlog)
target_include_directories(nask_parse PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/bnfc
  ${CMAKE_CURRENT_SOURCE_DIR}/incbin
  ${CMAKE_CURRENT_SOURCE_DIR}/jsoncons/include)
set_property(TARGET nask_parse PROPERTY CXX_STANDARD 17)

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

set(THREADS_PREFER_PTHREAD_FLAG ON)

# debug
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -gstabs")

add_executable(opennask ${opennask_SRCS})
target_link_libraries(opennask PRIVATE Threads::Threads parasol spdlog::spdlog)

set_property(TARGET opennask PROPERTY CXX_STANDARD 17)
set_property(TARGET opennask PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET opennask PROPERTY APPEND_STRING PROPERTY COMPILE_FLAGS -Wno-missing-braces)
