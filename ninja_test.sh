#!/bin/bash

# * testing logs
# tail -F build/Testing/Temporary/LastTest.log
#
./ninja_build.sh
cd build

if [ $# -eq 0 ]; then
    ctest;
else
    ctest -R $@;
fi
