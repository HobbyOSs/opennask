#!/bin/bash

# 1 line comments and opecode 'DB'
# 2 line source and opecode 'DB'
# 3 RESB command
# 4 Assembly to create img, "Hello, World"
# 5 line comments and opecode 'DW'
# 6 Day1 assembly file
# 7 Day2 assembly file
# 8 Day3 assembly file + 'JC', 'JE'
for n in $(seq 1 8)
do
    ./src/opennask data/0${n}_nasfile.nas data/0${n}_nasfile.img
    od -t x1 data/0${n}_nasfile.img > data/artifacts/0${n}_nasfile.txt
    ./fatlib/examples/fdstat data/0${n}_nasfile.img
    cd data && md5sum -c 0${n}_nasfile.hash && cd ../
done

# Test each projects
# 01_day |
# 02_day |
# 03_day | a~h
for f in `find ./projects -name \*.hash | sort | grep -v wine`
do
    echo "md5sum -c ${f%.*}.hash"
    md5sum -c ${f%.*}.hash
done
