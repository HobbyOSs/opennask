#!/bin/bash

rm -rf ./build
cmake -H. -Bbuild -G "Ninja"
cmake --build build
cd build && ctest && cd ..
