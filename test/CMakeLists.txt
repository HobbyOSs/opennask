message(STATUS "Entering directory test/")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

# debug
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# general settings
include_directories(
  ${ASMJIT_DIR}/src
  ${root_SOURCE_DIR}/matchit.cpp/include
  ${root_SOURCE_DIR}/parasol
  ${root_SOURCE_DIR}/spdlog/include
  ${root_SOURCE_DIR}/src
  ${root_SOURCE_DIR}/src/bnfc
  ${root_SOURCE_DIR}/src/coffi
  ${root_SOURCE_DIR}/src/incbin
  ${root_SOURCE_DIR}/src/jsoncons/include
  ${gtest_SOURCE_DIR}
  ${gtest_SOURCE_DIR}/include
)

# http://google.github.io/googletest/quickstart-cmake.html
include(GoogleTest)

set(nask_parse_bnfc_SRCS
  ../src/bnfc/skeleton.cc
  ../src/bnfc/absyn.cc
  ../src/bnfc/parser.cc
  ../src/bnfc/printer.cc
  ../src/bnfc/lexer.cc
  ../src/bnfc/driver.cc
  ../src/bnfc/buffer.cc)

set(nask_parse_front_SRCS
  ../src/front_end/front_config.cc
  ../src/front_end/front_inst.cc
  ../src/front_end/front_inst_add.cc
  ../src/front_end/front_inst_jmp.cc
  ../src/front_end/front_inst_no_param.cc
  ../src/front_end/front_inst_pseudo.cc
  ../src/front_end/front_memory_addr.cc
  ../src/front_end/front_parse.cc)

set(nask_parse_pass1_SRCS
  ../src/pass1/pass1_inst_add.cc
  ../src/pass1/pass1_inst_jmp.cc
  ../src/pass1/pass1_inst_no_param.cc
  ../src/pass1/pass1_strategy.cc)

set(nask_parse_SRCS
  ${nask_parse_bnfc_SRCS}
  ${nask_parse_front_SRCS}
  ${nask_parse_pass1_SRCS}
  ../src/bin_util.cc
  ../src/demangle.cpp
  ../src/para_token.cc
  ../src/mod_rm.cpp
  ../src/string_util.cpp
  ../src/x86.cc
  ../src/x86_prefix.cc
  ../src/object_file_writer.cc)

# テストで使われる共通のソースコードはObject Libraryを使い再コンパイルしないようにする
# Object Libraryにspdlogを含ませないとテスト時のdebugログが出なかった
add_library(nask_parse_objects OBJECT ${nask_parse_SRCS})
target_link_libraries(nask_parse_objects PRIVATE spdlog::spdlog)
target_compile_features(nask_parse_objects PRIVATE cxx_std_17)

########### next target ###############

set(parsertest_SRCS inst_suite.cpp)
add_executable(inst_suite_test ${parsertest_SRCS})
target_link_libraries(inst_suite_test PRIVATE
  nask_parse_objects
  parasol
  spdlog::spdlog
  asmjit::asmjit
  Threads::Threads
  GTest::gtest_main)

if(WITH_BACKWARDS_CPP)
  target_link_libraries(inst_suite_test PUBLIC -lbfd)
  target_link_libraries(inst_suite_test PUBLIC backward)
endif()

target_compile_features(inst_suite_test PRIVATE cxx_std_17)
gtest_discover_tests(inst_suite_test)

########### next target ###############

# lexer, parser testing
# `ctest -R exp_suite_test`
set(parsertest_SRCS
  exp_suite.cpp)

add_executable(exp_suite_test ${parsertest_SRCS})
target_link_libraries(exp_suite_test PRIVATE
  nask_parse_objects
  Threads::Threads
  spdlog::spdlog
  asmjit::asmjit
  parasol
  GTest::gtest_main)
if(WITH_BACKWARDS_CPP)
  target_link_libraries(exp_suite_test PUBLIC -lbfd)
  target_link_libraries(exp_suite_test PUBLIC backward)
endif()

target_compile_features(exp_suite_test PRIVATE cxx_std_17)
gtest_discover_tests(exp_suite_test)

# `ctest -R modrmtest`
set(modrmtest_SRCS
    modrmtest.cpp
    ../src/tinyexpr.c
    ../src/nask_utility.cpp
    ../src/mod_rm.cpp
    ../src/string_util.cpp)

add_executable(modrmtest ${modrmtest_SRCS})
target_link_libraries(modrmtest PRIVATE
  parasol
  spdlog::spdlog
  Threads::Threads
  GTest::gtest_main)

target_compile_features(modrmtest PRIVATE cxx_std_17)
gtest_discover_tests(modrmtest)

# `ctest -R x86_table_test`
set(x86_table_test_SRCS
  x86_table_test.cpp
  ../src/x86.cc
  ../src/x86_prefix.cc
  ../src/demangle.cpp
  ../src/para_token.cc)

add_executable(x86_table_test ${x86_table_test_SRCS})
target_link_libraries(x86_table_test PRIVATE
  parasol spdlog::spdlog Threads::Threads GTest::gtest_main)

target_compile_features(x86_table_test PRIVATE cxx_std_17)
gtest_discover_tests(x86_table_test)

# `ctest -R para_token_test`
set(para_token_test_SRCS
  para_token_test.cpp
  ../src/demangle.cpp
  ../src/para_token.cc)

add_executable(para_token_test ${para_token_test_SRCS})
target_link_libraries(para_token_test PRIVATE
  parasol spdlog::spdlog Threads::Threads GTest::gtest_main)

target_compile_features(para_token_test PRIVATE cxx_std_17)
gtest_discover_tests(para_token_test)

# `ctest -R pass1_test`
set(pass1_test_SRCS
  pass1_test.cpp)

add_executable(pass1_test ${pass1_test_SRCS})
target_link_libraries(pass1_test PRIVATE
  nask_parse_objects
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)

target_compile_features(pass1_test PRIVATE cxx_std_17)
gtest_discover_tests(pass1_test)

# `ctest -R day01`
set(day01test_SRCS
  day01test.cpp)

add_executable(day01test ${day01test_SRCS})
target_link_libraries(day01test PRIVATE
  nask_parse_objects
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)
if(WITH_BACKWARDS_CPP)
  target_link_libraries(day01test PUBLIC -lbfd)
  target_link_libraries(day01test PUBLIC backward)
endif()

target_compile_features(day01test PRIVATE cxx_std_17)
gtest_discover_tests(day01test)

# `ctest -R day02`
set(day02test_SRCS
  day02test.cpp)

add_executable(day02test ${day02test_SRCS})
target_link_libraries(day02test PRIVATE
  nask_parse_objects
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)
if(WITH_BACKWARDS_CPP)
  target_link_libraries(day02test PUBLIC -lbfd)
  target_link_libraries(day02test PUBLIC backward)
endif()

target_compile_features(day02test PRIVATE cxx_std_17)
gtest_discover_tests(day02test)

# `ctest -R day03`
set(day03test_SRCS
  day03test.cpp)

add_executable(day03test ${day03test_SRCS})
target_link_libraries(day03test PRIVATE
  nask_parse_objects
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)
if(WITH_BACKWARDS_CPP)
  target_link_libraries(day03test PUBLIC -lbfd)
  target_link_libraries(day03test PUBLIC backward)
endif()

target_compile_features(day03test PRIVATE cxx_std_17)
gtest_discover_tests(day03test)

# `ctest -R day04`
set(day04test_SRCS
  day04test.cpp)

add_executable(day04test ${day04test_SRCS})
target_link_libraries(day04test PRIVATE
  nask_parse_objects
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)
if(WITH_BACKWARDS_CPP)
  target_link_libraries(day04test PUBLIC -lbfd)
  target_link_libraries(day04test PUBLIC backward)
endif()

target_compile_features(day04test PRIVATE cxx_std_17)
gtest_discover_tests(day04test)

# `ctest -R day05`
set(day05test_SRCS
  day05test.cpp)

add_executable(day05test ${day05test_SRCS})
target_link_libraries(day05test PRIVATE
  nask_parse_objects
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)
if(WITH_BACKWARDS_CPP)
  target_link_libraries(day05test PUBLIC -lbfd)
  target_link_libraries(day05test PUBLIC backward)
endif()

target_compile_features(day05test PRIVATE cxx_std_17)
gtest_discover_tests(day05test)

# `ctest -R day06`
set(day06test_SRCS
  day06test.cpp)

add_executable(day06test ${day06test_SRCS})
target_link_libraries(day06test PRIVATE
  nask_parse_objects
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)
if(WITH_BACKWARDS_CPP)
  target_link_libraries(day06test PUBLIC -lbfd)
  target_link_libraries(day06test PUBLIC backward)
endif()

target_compile_features(day06test PRIVATE cxx_std_17)
gtest_discover_tests(day06test)

# `ctest -R day09`
set(day09test_SRCS
  day09test.cpp)

add_executable(day09test ${day09test_SRCS})
target_link_libraries(day09test PRIVATE
  nask_parse_objects
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)
if(WITH_BACKWARDS_CPP)
  target_link_libraries(day09test PUBLIC -lbfd)
  target_link_libraries(day09test PUBLIC backward)
endif()

target_compile_features(day09test PRIVATE cxx_std_17)
gtest_discover_tests(day09test)

# `ctest -R day12`
add_test(NAME create_day12_file
  COMMAND $<TARGET_FILE:nask_parse> ${root_SOURCE_DIR}/projects/12_day/harib09a/naskfunc.nas ${root_BINARY_DIR}/12_day_harib09a_naskfunc.img)
add_test(NAME day12test
  COMMAND sha256sum -c ${root_SOURCE_DIR}/test/day12_naskfunc.sha256
  WORKING_DIRECTORY ${root_SOURCE_DIR})
set_tests_properties(day12test
  PROPERTIES DEPENDS create_day12_file)
