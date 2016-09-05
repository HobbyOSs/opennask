message(STATUS "Entering directory .")
cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(intel2gas CXX)

set(intel2gas_VERSION 1.3.3)
set(PACKAGE ON CACHE STRING "intel2gas" FORCE)
set(PACKAGE_NAME ON CACHE STRING "intel2gas" FORCE)
set(PACKAGE_STRING ON CACHE STRING "intel2gas 1.3.3" FORCE)
set(PACKAGE_TARNAME ON CACHE STRING "intel2gas" FORCE)
set(PACKAGE_VERSION ON CACHE STRING "1.3.3" FORCE)
set(PACKAGE_BUGREPORT ON CACHE STRING "mikko.tiihonen@hut.fi" FORCE)

set(intel2gas_SRCS
    strhandle.cc
    loaddata.cc
    intel2gas.cc
    strhandle.h
    loaddata.h
    intel2gas.h)

add_executable(intel2gas ${intel2gas_SRCS})
set_property(TARGET intel2gas PROPERTY CXX_STANDARD 11)
set_property(TARGET intel2gas PROPERTY CXX_STANDARD_REQUIRED ON)

if (DEFINED DESTDIR)
  add_definitions( -DRESOURCE_PATH="${DESTDIR}/share/intel2gas/" )
else()
  add_definitions( -DRESOURCE_PATH="/usr/local/share/intel2gas/" )
endif()
