# opennask [![Build Status](https://travis-ci.org/Hiroyuki-Nagata/opennask.svg?branch=master)](https://travis-ci.org/Hiroyuki-Nagata/opennask)
An 80x86 assembler like MASM/NASM for the tiny OS

# build
* You need to install cmake
```
$ mkdir build
$ cd build

# if you use msys2
$ cmake -G "MSYS Makefiles" -DCMAKE_MAKE_PROGRAM=/usr/bin/make ..

# if you use Unix environment
$ cmake -G "Unix Makefiles" -DCMAKE_MAKE_PROGRAM=/usr/bin/make ..
$ make
```
