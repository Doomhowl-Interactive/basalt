#!/bin/sh
set -xe

cd build

gcc ../src/tooling/embedder.c -o embedder
./embedder ../assets ../src/assets_custom.dat.c

gcc -ggdb -Werror -DBASALT_DEBUG -DARCHAEO ../src/basalt_*.c ../src/bullet_game.c ../src/xorg_basalt.c ../src/assets_custom.dat.c -lX11 -lm -lXext -o basalt_linux_archaeo.x11 
cd ..

./build/basalt_linux_archaeo.x11
