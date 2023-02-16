message(STATUS "Entering directory test/")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
find_package(CppUTest REQUIRED)

########### next target ###############

# memory leak detection, see "Conflicts with operator new macros (STL!)"
# https://cpputest.github.io/manual.html
add_definitions(-DCPPUTEST_USE_MEM_LEAK_DETECTION)

# general settings
include_directories(${root_SOURCE_DIR}/parasol
  ${root_SOURCE_DIR}/src
  ${root_SOURCE_DIR}/src/bnfc
  ${root_SOURCE_DIR}/src/incbin
  ${root_SOURCE_DIR}/src/jsoncons/include
  ${CPPUTEST_INCLUDE_DIR}
  ${root_SOURCE_DIR}/spdlog/include
  ${matchit_SOURCE_DIR}/include)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# lexer, parser testing
# `ctest -R parsertest`
set(parsertest_SRCS
  exp_suite.cpp
  ../src/bnfc/skeleton.cc
  ../src/bnfc/absyn.cc
  ../src/bnfc/parser.cc
  ../src/bnfc/printer.cc
  ../src/bnfc/lexer.cc
  ../src/bnfc/driver.cc
  ../src/bnfc/buffer.cc
  ../src/front_end.cc
  ../src/pass1_strategy.cc
  ../src/bin_util.cc
  ../src/demangle.cpp
  ../src/para_token.cc
  ../src/mod_rm.cpp
  ../src/string_util.cpp
  ../src/label.cpp)

add_executable(parsertest ${parsertest_SRCS} ${BACKWARD_ENABLE})
target_link_libraries(parsertest PRIVATE
  parasol spdlog::spdlog Threads::Threads ${CPPUTEST_LIBRARY})
add_backward(parsertest)

set_target_properties(parsertest PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS ON)

add_test(
  NAME parsertest
  COMMAND $<TARGET_FILE:parsertest>
)

set_tests_properties(parsertest PROPERTIES ATTACHED_FILES_ON_FAIL 0)

# `ctest -R modrmtest`
set(modrmtest_SRCS
    modrmtest.cpp
    ../src/tinyexpr.c
    ../src/nask_utility.cpp
    ../src/mod_rm.cpp
    ../src/string_util.cpp)

add_executable(modrmtest ${modrmtest_SRCS})
target_link_libraries(modrmtest PRIVATE
  parasol spdlog::spdlog Threads::Threads ${CPPUTEST_LIBRARY})

set_property(TARGET modrmtest PROPERTY CXX_STANDARD 17)
set_property(TARGET modrmtest PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(
  NAME modrmtest
  COMMAND $<TARGET_FILE:modrmtest>
)

set_tests_properties(modrmtest PROPERTIES ATTACHED_FILES_ON_FAIL 0)

# `ctest -R x86_table_test`
set(x86_table_test_SRCS
    x86_table_test.cpp)

add_executable(x86_table_test ${x86_table_test_SRCS})
target_link_libraries(x86_table_test PRIVATE
  parasol spdlog::spdlog Threads::Threads ${CPPUTEST_LIBRARY})

set_property(TARGET x86_table_test PROPERTY CXX_STANDARD 17)
set_property(TARGET x86_table_test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(
  NAME x86_table_test
  COMMAND $<TARGET_FILE:x86_table_test>
)

set_tests_properties(x86_table_test PROPERTIES ATTACHED_FILES_ON_FAIL 0)

# `ctest -R day01`
set(day01test_SRCS
  day01test.cpp
  ../src/bnfc/skeleton.cc
  ../src/bnfc/absyn.cc
  ../src/bnfc/parser.cc
  ../src/bnfc/printer.cc
  ../src/bnfc/lexer.cc
  ../src/bnfc/driver.cc
  ../src/bnfc/buffer.cc
  ../src/front_end.cc
  ../src/pass1_strategy.cc
  ../src/bin_util.cc
  ../src/demangle.cpp
  ../src/para_token.cc
  ../src/mod_rm.cpp
  ../src/string_util.cpp
  ../src/label.cpp)

add_executable(day01test ${day01test_SRCS} ${BACKWARD_ENABLE})
target_link_libraries(day01test PRIVATE
  parasol spdlog::spdlog Threads::Threads ${CPPUTEST_LIBRARY})
add_backward(day01test)

set_property(TARGET day01test PROPERTY CXX_STANDARD 17)
set_property(TARGET day01test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(
  NAME day01test
  COMMAND $<TARGET_FILE:day01test>
)

set_tests_properties(day01test PROPERTIES ATTACHED_FILES_ON_FAIL 0)

# `ctest -R day02`
set(day02test_SRCS
  day02test.cpp
  ../src/bnfc/skeleton.cc
  ../src/bnfc/absyn.cc
  ../src/bnfc/parser.cc
  ../src/bnfc/printer.cc
  ../src/bnfc/lexer.cc
  ../src/bnfc/buffer.cc
  ../src/bnfc/driver.cc
  ../src/front_end.cc
  ../src/pass1_strategy.cc
  ../src/bin_util.cc
  ../src/demangle.cpp
  ../src/para_token.cc
  ../src/mod_rm.cpp
  ../src/string_util.cpp
  ../src/label.cpp)

add_executable(day02test ${day02test_SRCS} ${BACKWARD_ENABLE})
target_link_libraries(day02test PRIVATE
  parasol spdlog::spdlog Threads::Threads ${CPPUTEST_LIBRARY})
add_backward(day02test)

set_property(TARGET day02test PROPERTY CXX_STANDARD 17)
set_property(TARGET day02test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(
  NAME day02test
  COMMAND $<TARGET_FILE:day02test>
)

set_tests_properties(day02test PROPERTIES ATTACHED_FILES_ON_FAIL 0)

# `ctest -R day03`
set(day03test_SRCS
  day03test.cpp
  ../src/bnfc/skeleton.cc
  ../src/bnfc/absyn.cc
  ../src/bnfc/parser.cc
  ../src/bnfc/printer.cc
  ../src/bnfc/lexer.cc
  ../src/bnfc/buffer.cc
  ../src/bnfc/driver.cc
  ../src/front_end.cc
  ../src/pass1_strategy.cc
  ../src/bin_util.cc
  ../src/demangle.cpp
  ../src/para_token.cc
  ../src/mod_rm.cpp
  ../src/string_util.cpp
  ../src/label.cpp)

add_executable(day03test ${day03test_SRCS} ${BACKWARD_ENABLE})
target_link_libraries(day03test PRIVATE
  parasol spdlog::spdlog Threads::Threads ${CPPUTEST_LIBRARY})
add_backward(day03test)

set_property(TARGET day03test PROPERTY CXX_STANDARD 17)
set_property(TARGET day03test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(
  NAME day03test
  COMMAND $<TARGET_FILE:day03test>
)

set_tests_properties(day03test PROPERTIES ATTACHED_FILES_ON_FAIL 0)

# `ctest -R day04`
set(day04test_SRCS
    day04test.cpp
    ../src/tinyexpr.c
    ../src/nask_utility.cpp
    ../src/mod_rm.cpp
    ../src/string_util.cpp)

add_executable(day04test ${day04test_SRCS})
target_link_libraries(day04test PRIVATE
  parasol spdlog::spdlog Threads::Threads ${CPPUTEST_LIBRARY})

set_property(TARGET day04test PROPERTY CXX_STANDARD 11)
set_property(TARGET day04test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(
  NAME day04test
  COMMAND $<TARGET_FILE:day04test>
)

set_tests_properties(day04test PROPERTIES ATTACHED_FILES_ON_FAIL 0)

# `ctest -R day05`
set(day05test_SRCS
    day05test.cpp
    ../src/tinyexpr.c
    ../src/nask_utility.cpp
    ../src/mod_rm.cpp
    ../src/string_util.cpp)

add_executable(day05test ${day05test_SRCS})
target_link_libraries(day05test PRIVATE
  parasol spdlog::spdlog Threads::Threads ${CPPUTEST_LIBRARY})

set_property(TARGET day05test PROPERTY CXX_STANDARD 11)
set_property(TARGET day05test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(
  NAME day05test
  COMMAND $<TARGET_FILE:day05test>
)

set_tests_properties(day05test PROPERTIES ATTACHED_FILES_ON_FAIL 0)
