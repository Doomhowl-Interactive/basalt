#!/bin/sh
echo "Building..."
mkdir -p build_xorg
cd build_xorg

gcc ../src/basalt_*.c ../src/xorg_basalt.c ../src/assets_custom.dat.c

# test run
./a.out

cd ..
