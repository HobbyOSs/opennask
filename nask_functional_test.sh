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
    ./fatlib/examples/fdstat data/0${n}_nasfile.img
    cd data && md5sum -c 0${n}_nasfile.hash && cd ../
done

# 8 create FAT12
#./src/opennask --with-fat12 data/08_nasfile.nas data/08_nasfile.img
#./fatlib/examples/fdstat data/08_nasfile.img
#cd data && md5sum -c 0${n}_nasfile.hash && cd ../
