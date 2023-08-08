message(STATUS "Entering directory test/")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

set(THREADS_PREFER_PTHREAD_FLAG ON)
# debug
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -gstabs")
find_package(Threads REQUIRED)
#find_package(GTest REQUIRED)

# general settings
include_directories(${root_SOURCE_DIR}/parasol
  ${root_SOURCE_DIR}/src
  ${root_SOURCE_DIR}/src/bnfc
  ${root_SOURCE_DIR}/src/incbin
  ${ASMJIT_DIR}/src
  ${root_SOURCE_DIR}/src/jsoncons/include
  ${root_SOURCE_DIR}/spdlog/include
  ${root_SOURCE_DIR}/matchit.cpp/include
  ${gtest_SOURCE_DIR}/include
  ${gtest_SOURCE_DIR})

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
  ../src/front_end/front_parse.cc
  ../src/front_end/front_memory_addr.cc
  ../src/front_end/front_inst.cc
  ../src/front_end/front_inst_jmp.cc
  ../src/front_end/front_inst_pseudo.cc)

set(nask_parse_SRCS
  ${nask_parse_bnfc_SRCS}
  ${nask_parse_front_SRCS}
  ../src/pass1_strategy.cc
  ../src/bin_util.cc
  ../src/demangle.cpp
  ../src/para_token.cc
  ../src/mod_rm.cpp
  ../src/string_util.cpp
  ../src/label.cpp
  ../src/x86.cc)

set(parsertest_SRCS
  inst_suite.cpp
  ${nask_parse_SRCS})

add_executable(inst_suite_test ${parsertest_SRCS} ${BACKWARD_ENABLE})
target_link_libraries(inst_suite_test PRIVATE
  parasol
  spdlog::spdlog
  asmjit::asmjit
  Threads::Threads
  GTest::gtest_main)
add_backward(inst_suite_test)

set_property(TARGET inst_suite_test PROPERTY CXX_STANDARD 17)
set_property(TARGET inst_suite_test PROPERTY CXX_STANDARD_REQUIRED ON)
gtest_discover_tests(inst_suite_test)

########### next target ###############

# lexer, parser testing
# `ctest -R parsertest`
set(parsertest_SRCS
  exp_suite.cpp
  ${nask_parse_SRCS})

add_executable(parsertest ${parsertest_SRCS} ${BACKWARD_ENABLE})
target_link_libraries(parsertest PRIVATE
  Threads::Threads
  spdlog::spdlog
  asmjit::asmjit
  parasol
  GTest::gtest_main)
add_backward(parsertest)

set_property(TARGET parsertest PROPERTY CXX_STANDARD 17)
set_property(TARGET parsertest PROPERTY CXX_STANDARD_REQUIRED ON)
gtest_discover_tests(parsertest)

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

set_property(TARGET modrmtest PROPERTY CXX_STANDARD 17)
set_property(TARGET modrmtest PROPERTY CXX_STANDARD_REQUIRED ON)
gtest_discover_tests(modrmtest)

# `ctest -R x86_table_test`
set(x86_table_test_SRCS
  x86_table_test.cpp
  ../src/x86.cc
  ../src/demangle.cpp
  ../src/para_token.cc)

add_executable(x86_table_test ${x86_table_test_SRCS})
target_link_libraries(x86_table_test PRIVATE
  parasol spdlog::spdlog Threads::Threads GTest::gtest_main)

set_property(TARGET x86_table_test PROPERTY CXX_STANDARD 17)
set_property(TARGET x86_table_test PROPERTY CXX_STANDARD_REQUIRED ON)
gtest_discover_tests(x86_table_test)

# `ctest -R para_token_test`
set(para_token_test_SRCS
  para_token_test.cpp
  ../src/demangle.cpp
  ../src/para_token.cc)

add_executable(para_token_test ${para_token_test_SRCS})
target_link_libraries(para_token_test PRIVATE
  parasol spdlog::spdlog Threads::Threads GTest::gtest_main)

set_property(TARGET para_token_test PROPERTY CXX_STANDARD 17)
set_property(TARGET para_token_test PROPERTY CXX_STANDARD_REQUIRED ON)
gtest_discover_tests(para_token_test)

# `ctest -R pass1_test`
set(pass1_test_SRCS
  pass1_test.cpp
  ${nask_parse_SRCS})

add_executable(pass1_test ${pass1_test_SRCS})
target_link_libraries(pass1_test PRIVATE
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)

set_property(TARGET pass1_test PROPERTY CXX_STANDARD 17)
set_property(TARGET pass1_test PROPERTY CXX_STANDARD_REQUIRED ON)
gtest_discover_tests(pass1_test)

# `ctest -R day01`
set(day01test_SRCS
  day01test.cpp
  ${nask_parse_SRCS})

add_executable(day01test ${day01test_SRCS} ${BACKWARD_ENABLE})
target_link_libraries(day01test PRIVATE
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)
add_backward(day01test)

set_property(TARGET day01test PROPERTY CXX_STANDARD 17)
set_property(TARGET day01test PROPERTY CXX_STANDARD_REQUIRED ON)
gtest_discover_tests(day01test)

# `ctest -R day02`
set(day02test_SRCS
  day02test.cpp
  ${nask_parse_SRCS})

add_executable(day02test ${day02test_SRCS} ${BACKWARD_ENABLE})
target_link_libraries(day02test PRIVATE
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)
add_backward(day02test)

set_property(TARGET day02test PROPERTY CXX_STANDARD 17)
set_property(TARGET day02test PROPERTY CXX_STANDARD_REQUIRED ON)
gtest_discover_tests(day02test)

# `ctest -R day03`
set(day03test_SRCS
  day03test.cpp
  ${nask_parse_SRCS})

add_executable(day03test ${day03test_SRCS} ${BACKWARD_ENABLE})
target_link_libraries(day03test PRIVATE
  parasol
  spdlog::spdlog
  Threads::Threads
  asmjit::asmjit
  GTest::gtest_main)
add_backward(day03test)

set_property(TARGET day03test PROPERTY CXX_STANDARD 17)
set_property(TARGET day03test PROPERTY CXX_STANDARD_REQUIRED ON)
gtest_discover_tests(day03test)

# `ctest -R day04`
set(day04test_SRCS
    day04test.cpp
    ../src/tinyexpr.c
    ../src/nask_utility.cpp
    ../src/mod_rm.cpp
    ../src/string_util.cpp)

add_executable(day04test ${day04test_SRCS})
target_link_libraries(day04test PRIVATE
  parasol spdlog::spdlog Threads::Threads GTest::gtest_main)

set_property(TARGET day04test PROPERTY CXX_STANDARD 11)
set_property(TARGET day04test PROPERTY CXX_STANDARD_REQUIRED ON)
gtest_discover_tests(day04test)

# `ctest -R day05`
set(day05test_SRCS
    day05test.cpp
    ../src/tinyexpr.c
    ../src/nask_utility.cpp
    ../src/mod_rm.cpp
    ../src/string_util.cpp)

add_executable(day05test ${day05test_SRCS})
target_link_libraries(day05test PRIVATE
  parasol spdlog::spdlog Threads::Threads GTest::gtest_main)

set_property(TARGET day05test PROPERTY CXX_STANDARD 11)
set_property(TARGET day05test PROPERTY CXX_STANDARD_REQUIRED ON)
gtest_discover_tests(day05test)
