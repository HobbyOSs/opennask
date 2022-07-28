#!/bin/bash


CMAKE=/usr/bin/cmake
if [ ! $(type -P ${CMAKE} 2>/dev/null) ]; then CMAKE=/usr/local/bin/cmake; fi
git submodule init
git submodule update
if [ ! -e build ]; then mkdir build; fi
cd build
$CMAKE --version
# show all options
# $CMAKE -LA ..

$CMAKE $CMAKE_OPT -G "Ninja" ..
ninja $@
