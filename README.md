# opennask [![Build Status](https://travis-ci.org/Hiroyuki-Nagata/opennask.svg?branch=master)](https://travis-ci.org/Hiroyuki-Nagata/opennask) [![Build status](https://ci.appveyor.com/api/projects/status/3y5g0uyrixr7ivgd?svg=true)](https://ci.appveyor.com/project/Hiroyuki-Nagata/opennask) [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)
An 80x86 assembler like MASM/NASM for the tiny OS

# build
* You need to install cmake
```
$ mkdir build
$ cd build

# if you use msys2
$ cmake -G "MSYS Makefiles" ..
$ make

# if you use Unix environment
$ cmake -G "Unix Makefiles" ..
$ make

# if you can use Ninja
$ cmake -G "Ninja" ..
$ ninja
```
