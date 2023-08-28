message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(opennask LANGUAGES CXX VERSION 1.0.3)
project(nask_parse LANGUAGES CXX VERSION 1.0.3)

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

# asmjit
set(ASMJIT_DIR ${root_SOURCE_DIR}/asmjit)
set(ASMJIT_STATIC TRUE)
add_compile_definitions(ASMJIT_NO_AARCH32)
add_compile_definitions(ASMJIT_NO_AARCH64)
add_compile_definitions(ASMJIT_NO_DEPRECATED)
add_compile_definitions(ASMJIT_NO_LOGGING)
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

set(nask_parse_pass1_SRCS
  pass1/pass1_inst_jmp.cc
  pass1/pass1_inst_no_param.cc
  pass1/pass1_strategy.cc)

set(nask_parse_SRCS
  nask_parse.cpp
  ${nask_parse_front_SRCS}
  front_end.hh
  ${nask_parse_pass1_SRCS}
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

# バイナリサイズ削減のため
add_custom_target(strip_target
    COMMAND strip nask_parse
    DEPENDS nask_parse
  )
# リリース時のみstrip
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_dependencies(strip_target nask_parse)
endif()

#
# Debianパッケージの設定
#
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  set(CPACK_GENERATOR "DEB")
  set(CPACK_PACKAGE_NAME "opennask")
  set(CPACK_PACKAGE_VERSION "1.0.3")
  set(CPACK_PACKAGE_CONTACT "idiotpanzer@gmail.com")
  set(CPACK_DEBIAN_FILE_NAME DEB-DEFAULT)
  set(CPACK_DEBIAN_PACKAGE_DEPENDS "libstdc++6")

  set(CPACK_SET_DESTDIR true)
  set(CPACK_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")
  install(TARGETS nask_parse DESTINATION /usr/bin)  ## いずれ入れ替える

  include(CPack)
endif()

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
  nask_parse_common.hh
  tinyexpr.c
  tinyexpr.h)

### versioning
add_definitions(-DOPENNASK_VERSION=\"${opennask_VERSION}\")

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
