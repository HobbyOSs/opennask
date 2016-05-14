#!/bin/bash

# 1 line comments and opecode 'DB'
./opennask data/01_nasfile.nas data/01_nasfile.img
cd data && md5sum -c 01_nasfile.hash && cd ../

# 2 line source and opecode 'DB'
./opennask data/02_nasfile.nas data/02_nasfile.img
cd data && md5sum -c 02_nasfile.hash && cd ../
