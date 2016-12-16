# opennask [![Build Status](https://travis-ci.org/HobbyOSs/opennask.svg?branch=master)](https://travis-ci.org/HobbyOSs/opennask) [![Build status](https://ci.appveyor.com/api/projects/status/3y5g0uyrixr7ivgd?svg=true)](https://ci.appveyor.com/project/HobbyOSs/opennask) [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](http://www.gnu.org/licenses/gpl-3.0)
An 80x86 assembler like MASM/NASM for the tiny OS

# Build
* You need to install cmake, and ninja / make
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

# Build osask project files

* You need to install mtools to build these files
* You need to install QEMU to test it

'wine' will be very helpful for you to wine debug original nask

## Build day 03, harib00i img

* Target names are formatted like `XX_day_haribxxx_{action}`
    * Actions are `sys`, `run`, `clean` and so on

```
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
