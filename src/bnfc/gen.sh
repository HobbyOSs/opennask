#!/bin/bash

bnfc -m -cpp_stl nask.cf
rm -f *.bak *.cc *.hh Makefile
rename 's/\.C$/.cc/' *.C
rename 's/\.H$/.hh/' *.H
rename 'y/A-Z/a-z/' *
rm -f *.c *.h
