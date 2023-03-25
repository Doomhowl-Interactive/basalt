#!/bin/sh
set -xe

echo "Building..."
mkdir -p build
cd build

# build embedder
gcc -ggdb ../src/tooling/embedder.c -o embedder
./embedder ../assets ../src/assets_custom.dat.c

# build archaeo
gcc -ggdb -Werror -DBASALT_DEBUG ../src/basalt_*.c ../src/xorg_basalt.c ../src/tooling/archaeo.c \
    -lX11 -lm -lXext -I ../src -o archaeo

# build editor
gcc -ggdb -Werror -DBASALT_DEBUG ../src/basalt_*.c \
    ../src/tooling/editor.c ../src/xorg_basalt.c ../src/assets_custom.dat.c \
    -lX11 -lm -lXext -o editor_linux.x11 -I ../src

cd ..
