#!/bin/bash
set -xe

echo "Building..."
mkdir -p build
cd build

# build embedder
if test -f "embedder"; then
    echo Using existing embedder...
else
    echo Building embedder...
    gcc -O3 ../src/tooling/embedder.c -o embedder
fi
./embedder ../assets ../src/assets_custom.dat.c

# build game
gcc -ggdb -Werror -O3 ../src/basalt_*.c ../src/bullet_*.c ../src/xorg_basalt.c ../src/assets_custom.dat.c \
        -lX11 -lm -lXext -o basalt_linux_rel.x11 

cd ..
