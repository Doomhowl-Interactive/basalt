#!/bin/bash
set -xe

mkdir -p build
cd build

gcc ../src/sdl2_basalt.c ../src/basalt_*.c ../src/bullet_*.c ../src/assets_custom.dat.c \
    -lSDL2 -lm -o sdl2_basalt.x11

cd ..
