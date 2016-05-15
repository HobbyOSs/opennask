#!/bin/bash

# 1 line comments and opecode 'DB'
# 2 line source and opecode 'DB'
# 3 RESB command
# 4 Assembly to create img, "Hello, World"
# 5 line comments and opecode 'DW'

for n in $(seq 1 5)
do
    ./opennask data/0${n}_nasfile.nas data/0${n}_nasfile.img
    cd data && md5sum -c 0${n}_nasfile.hash && cd ../
done
