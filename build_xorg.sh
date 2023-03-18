#!/bin/sh
set -xe

echo "Building..."
mkdir -p build
cd build

# build embedder
gcc ../src/tooling/embedder.c -o embedder
./embedder ../assets ../src/assets_custom.dat.c

# build game
gcc -ggdb -Werror -DBASALT_DEBUG ../src/basalt_*.c ../src/temple_game.c ../src/xorg_basalt.c ../src/assets_custom.dat.c -lX11 -lm -lXext -o basalt_linux.x11 

cd ..
