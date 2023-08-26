message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(opennask CXX)
set(opennask_VERSION \"1.0.3\")

# use lld
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-fuse-ld=lld" COMPILER_SUPPORTS_LLD)

if(COMPILER_SUPPORTS_LLD)
  execute_process(COMMAND ${CMAKE_CXX_COMPILER} -fuse-ld=lld -Wl,--version OUTPUT_VARIABLE stdout ERROR_QUIET)
  if("${stdout}" MATCHES "lld")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fuse-ld=lld")
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fuse-ld=lld")
  endif()
endif()

find_package(Threads REQUIRED)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

set(ASMJIT_DIR ${root_SOURCE_DIR}/asmjit)
include("${ASMJIT_DIR}/CMakeLists.txt")

include_directories(
  ${ASMJIT_DIR}/src
  ${root_SOURCE_DIR}/jsoncons/include
  ${root_SOURCE_DIR}/matchit.cpp/include
  ${root_SOURCE_DIR}/parasol
  ${root_SOURCE_DIR}/spdlog/include
  ${root_SOURCE_DIR}/src
  ${root_SOURCE_DIR}/src/coffi
  ${root_SOURCE_DIR}/src/incbin
)

########### next target ###############

set(makefont_SRCS
  makefont.c)

add_executable(makefont ${makefont_SRCS})

########### next target ###############

set(bin2obj_SRCS
  bin2obj.c)

add_executable(bin2obj ${bin2obj_SRCS})

########### next target ###############

set(nask_parse_bnfc_SRCS
  bnfc/skeleton.cc
  bnfc/absyn.cc
  bnfc/parser.cc
  bnfc/printer.cc
  bnfc/lexer.cc
  bnfc/driver.cc
  bnfc/buffer.cc)

set(nask_parse_front_SRCS
  front_end/front_config.cc
  front_end/front_inst.cc
  front_end/front_inst_jmp.cc
  front_end/front_inst_no_param.cc
  front_end/front_inst_pseudo.cc
  front_end/front_memory_addr.cc
  front_end/front_parse.cc)

set(nask_parse_SRCS
  nask_parse.cpp
  ${nask_parse_front_SRCS}
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
  bin_util.cc
  bin_util.hh
  x86.cc
  x86.hh
  object_file_writer.cc
  object_file_writer.hh
  ${nask_parse_bnfc_SRCS}
  incbin/incbin.h
)

if (WIN32)
  LIST(APPEND nask_parse_SRCS getopt.h getopt.c)
  add_definitions(-Dstrcasecmp=_stricmp)
endif()

add_executable(nask_parse
  ${nask_parse_SRCS})
target_link_libraries(nask_parse PRIVATE
  Threads::Threads
  spdlog::spdlog
  asmjit::asmjit
)
target_include_directories(nask_parse PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/bnfc
  ${CMAKE_CURRENT_SOURCE_DIR}/incbin
  ${CMAKE_CURRENT_SOURCE_DIR}/jsoncons/include
  ${CMAKE_CURRENT_SOURCE_DIR}/coffi
)
target_compile_features(nask_parse PRIVATE cxx_std_17)

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
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

add_executable(opennask ${opennask_SRCS})
target_link_libraries(opennask
  PRIVATE
  Threads::Threads parasol spdlog::spdlog)

target_compile_features(opennask PRIVATE cxx_std_17)
set_property(TARGET opennask PROPERTY APPEND_STRING PROPERTY COMPILE_FLAGS -Wno-missing-braces)
