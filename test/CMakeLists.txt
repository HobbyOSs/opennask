message(STATUS "Entering directory test/")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
find_package(CppUTest REQUIRED)

########### next target ###############

# general settings
include_directories(${root_SOURCE_DIR}/parasol)
include_directories(${root_SOURCE_DIR}/src)
include_directories(${CPPUTEST_INCLUDE_DIR})

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# general testing
set(alltest_SRCS
    alltest.cpp
    nask_suite.cpp
    tinyexpr_suite.cpp
    inst_suite.cpp
    ../src/tinyexpr.c
    ../src/nask_utility.cpp
    ../src/mod_rm.cpp
    ../src/string_util.cpp)

add_executable(alltest ${alltest_SRCS})
target_link_libraries(alltest parasol Threads::Threads ${CPPUTEST_LIBRARY})

set_property(TARGET alltest PROPERTY CXX_STANDARD 11)
set_property(TARGET alltest PROPERTY CXX_STANDARD_REQUIRED ON)

add_test(
  NAME alltest
  COMMAND $<TARGET_FILE:alltest>
)

set_tests_properties(alltest PROPERTIES ATTACHED_FILES_ON_FAIL 0)

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
