#!/bin/bash
#
# script for cross compile i686-w64-mingw32
#
./bootstrap
./configure --prefix=/usr/i686-w64-mingw32           \
--build=x86_64-unknown-linux-gnu                     \
--host=i686-w64-mingw32                              \
--target=i686-w64-mingw32                            \
--disable-silent-rules
make
