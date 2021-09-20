message(STATUS "Entering directory test/")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
find_package(CppUTest REQUIRED)

########### next target ###############

# general settings
include_directories(${root_SOURCE_DIR}/parasol)
include_directories(${root_SOURCE_DIR}/src)
include_directories(${root_SOURCE_DIR}/src/bnfc)
include_directories(${CPPUTEST_INCLUDE_DIR})

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
  ../src/bnfc/buffer.cc
  ../src/driver.cc
  ../src/demangle.cpp
  ../src/para_token.cc
  ../src/para_token.hh)

add_executable(parsertest ${parsertest_SRCS})
target_link_libraries(parsertest Threads::Threads ${CPPUTEST_LIBRARY})

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
target_link_libraries(modrmtest parasol Threads::Threads ${CPPUTEST_LIBRARY})

set_property(TARGET modrmtest PROPERTY CXX_STANDARD 11)
set_property(TARGET modrmtest PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(
  NAME modrmtest
  COMMAND $<TARGET_FILE:modrmtest>
)

set_tests_properties(modrmtest PROPERTIES ATTACHED_FILES_ON_FAIL 0)

# `ctest -R day01`
set(day01test_SRCS
  day01test.cpp
  ../src/bnfc/skeleton.cc
  ../src/bnfc/absyn.cc
  ../src/bnfc/parser.cc
  ../src/bnfc/printer.cc
  ../src/bnfc/lexer.cc
  ../src/bnfc/buffer.cc
  ../src/driver.cc
  ../src/demangle.cpp
  ../src/para_token.cc
  ../src/para_token.hh)

add_executable(day01test ${day01test_SRCS} ${BACKWARD_ENABLE})
target_link_libraries(day01test Threads::Threads ${CPPUTEST_LIBRARY})
add_backward(day01test)

set_property(TARGET day01test PROPERTY CXX_STANDARD 17)
set_property(TARGET day01test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(
  NAME day01test
  COMMAND $<TARGET_FILE:day01test>
)

set_tests_properties(day01test PROPERTIES ATTACHED_FILES_ON_FAIL 0)

# `ctest -R day04`
set(day04test_SRCS
    day04test.cpp
    ../src/tinyexpr.c
    ../src/nask_utility.cpp
    ../src/mod_rm.cpp
    ../src/string_util.cpp)

add_executable(day04test ${day04test_SRCS})
target_link_libraries(day04test parasol Threads::Threads ${CPPUTEST_LIBRARY})

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
target_link_libraries(day05test parasol Threads::Threads ${CPPUTEST_LIBRARY})

set_property(TARGET day05test PROPERTY CXX_STANDARD 11)
set_property(TARGET day05test PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(
  NAME day05test
  COMMAND $<TARGET_FILE:day05test>
)

set_tests_properties(day05test PROPERTIES ATTACHED_FILES_ON_FAIL 0)
