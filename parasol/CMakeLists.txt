message(STATUS "Entering directory parasol/")

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


########### next target ###############

set(samples/statementsample_SRCS ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)


########### next target ###############

set(samples/expressionsample_SRCS ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)


########### next target ###############

set(samples/tokenizersample_SRCS ${CMAKE_CURRENT_BINARY_DIR}/dummy.cpp)

