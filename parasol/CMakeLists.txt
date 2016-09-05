message(STATUS "Entering directory parasol/")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

########### next target ###############

set(parasol_STAT_SRCS
    ParaDefs.cc
    ParaTokenTable.cc
    ParaToken.cc
    ParaTokenizer.cc
    ParaValue.cc
    ParaObject.cc
    ParaOperator.cc
    ParaExpression.cc
    ParaSymbolTable.cc
    ParaStatement.cc
    ParaFunction.cc
    ParaBuiltinFunction.cc
    ParaPackage.cc
    ParaParser.cc
    ParaStandardLibrary.cc
    ParaContainerLibrary.cc
    ParaMathLibrary.cc)

set(parasol_HEADERS
    ParaDefs.hh
    ParaTokenTable.hh
    ParaToken.hh
    ParaTokenizer.hh
    ParaValue.hh
    ParaObject.hh
    ParaOperator.hh
    ParaExpression.hh
    ParaSymbolTable.hh
    ParaStatement.hh
    ParaFunction.hh
    ParaBuiltinFunction.hh
    ParaPackage.hh
    ParaParser.hh
    ParaStandardLibrary.hh
    ParaContainerLibrary.hh
    ParaMathLibrary.hh)

add_library(parasol STATIC ${parasol_STAT_SRCS})
set_property(TARGET parasol PROPERTY CXX_STANDARD 11)
set_property(TARGET parasol PROPERTY CXX_STANDARD_REQUIRED ON)

########### next target ###############

set(samples_statementsample_SRCS samples/statementsample.cc)
include_directories(${PROJECT_SOURCE_DIR}/parasol)
add_executable(samples_statementsample ${samples_statementsample_SRCS} ${parasol_HEADERS})
target_link_libraries(samples_statementsample parasol)
set_property(TARGET samples_statementsample PROPERTY CXX_STANDARD 11)
set_property(TARGET samples_statementsample PROPERTY CXX_STANDARD_REQUIRED ON)

########### next target ###############

set(samples_expressionsample_SRCS samples/expressionsample.cc)
include_directories(${PROJECT_SOURCE_DIR}/parasol)
add_executable(samples_expressionsample ${samples_expressionsample_SRCS} ${parasol_HEADERS})
target_link_libraries(samples_expressionsample parasol)
set_property(TARGET samples_expressionsample PROPERTY CXX_STANDARD 11)
set_property(TARGET samples_expressionsample PROPERTY CXX_STANDARD_REQUIRED ON)

########### next target ###############

set(samples_tokenizersample_SRCS samples/tokenizersample.cc)
include_directories(${PROJECT_SOURCE_DIR}/parasol)
add_executable(samples_tokenizersample ${samples_tokenizersample_SRCS} ${parasol_HEADERS})
target_link_libraries(samples_tokenizersample parasol)
set_property(TARGET samples_tokenizersample PROPERTY CXX_STANDARD 11)
set_property(TARGET samples_tokenizersample PROPERTY CXX_STANDARD_REQUIRED ON)
