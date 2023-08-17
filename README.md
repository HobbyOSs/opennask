# opennask [![Build Status](https://github.com/HobbyOSs/opennask/actions/workflows/cmake.yml/badge.svg)](https://github.com/HobbyOSs/opennask/actions/workflows/cmake.yml) [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)
An 80x86 assembler like MASM/NASM for the tiny OS

# Build (debian)
* You need to install cmake, and ninja / make
```
// example
# apt-get install cmake ninja-build clang clang++ lld libc6-dev-i386 linux-headers-generic bison flex libfl-dev

$ mkdir build
$ cd build

# if you use Unix environment
$ cmake -G "Unix Makefiles" ..
$ make

# if you can use Ninja
$ cmake -G Ninja ..
$ ninja

# or, you may want to specify compile option

# use backward.cpp for debugging
$ CMAKE_OPT="-DWITH_BACKWARDS_CPP=ON" ./ninja_build.sh

# clang is faster than gcc
$ CMAKE_OPT="-DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++" ./ninja_build.sh
  ```

# Build osask project files (debian)

* You need to install mtools to build these files
* You need to install QEMU (> 3.1) to test it

'wine' should be very useful for you to debug

## Build day 03, harib00i img (debian)

* Target names are formatted like `XX_day_haribxxx_{action}`
    * Actions are `sys`, `run`, `clean` and so on

```
# apt-get install qemu

$ cd opennask/build
$ ninja
$ ninja 03_day_harib00i_img
$ ninja 03_day_harib00i_run
```

You can use `make` command instead of `ninja`

## Screenshots

| day3 harib00j | day4 harib01a |
|---------------|---------------|
|![harib00j](https://raw.githubusercontent.com/HobbyOSs/opennask/master/harib00j.png)|![harib01a](https://raw.githubusercontent.com/HobbyOSs/opennask/master/harib01a.png)|

| day4 harib01d | day5 harib02a |
|---------------|---------------|
|![harib01d](https://raw.githubusercontent.com/HobbyOSs/opennask/master/harib01d.png)|![harib02a](https://raw.githubusercontent.com/HobbyOSs/opennask/master/harib02a.png)|

| day5 harib02e | day5 harib02h |
|---------------|---------------|
|![harib02e](https://raw.githubusercontent.com/HobbyOSs/opennask/master/harib02e.png)|![harib02h](https://raw.githubusercontent.com/HobbyOSs/opennask/master/harib02h.png)|

| day6 harib03e | day8 harib05d |
|---------------|---------------|
|![harib03e](https://raw.githubusercontent.com/HobbyOSs/opennask/master/harib03e.png)|![harib05d](https://raw.githubusercontent.com/HobbyOSs/opennask/master/harib05d.png)|

| day20 harib17a |
|----------------|
|![harib17a.png](https://raw.githubusercontent.com/HobbyOSs/opennask/master/harib17a.png)|
