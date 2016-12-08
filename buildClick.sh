#!/bin/bash       
# Make click with all neccessary elements

cd click-master

mkdir -p $(pwd)/installdir
./configure --enable-local --enable-multithread --disable-linuxmodule --prefix=$(pwd)/installdir
make clean
make elemlist
make install
