#!/bin/bash

bnfc -m -cpp_stl nask.cf
rm -f *.bak *.cc *.hh Makefile
rename 's/\.C$/.cc/' *.C
rename 's/\.H$/.hh/' *.H
rename 'y/A-Z/a-z/' *
rm -f *.c *.h

# sed -i 's/Absyn.H/absyn.hh/g'
find . -name \*.cc -or -name \*.hh -or -name \*.l -or -name \*.y | xargs sed -i 's/"\(.\+\)\.H"/"\L\1\.hh"/'

# https://github.com/BNFC/bnfc/issues/377
sed -i '6i #include<algorithm>' absyn.hh
sed -i 's/%pure_parser/%define api.pure/g' nask.y
make
