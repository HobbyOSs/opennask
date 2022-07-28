#!/bin/bash

rm -rf ./build
cmake -H. -Bbuild -G "Ninja" # -DCMAKE_BUILD_TYPE=Debug
cmake --build build
cd build && ctest && cd ..
