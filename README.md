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
