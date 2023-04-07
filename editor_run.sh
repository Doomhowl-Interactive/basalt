#!/bin/sh
# NOTE: Editor gets recompiled every time you run it.

echo "Building editor..."
mkdir -p build
cd build

# build embedder
gcc ../src/tooling/embedder.c -o embedder
./embedder ../assets ../src/assets_custom.dat.c

# build editor
rm -rf editor_linux.x11
gcc -ggdb -Werror ../src/basalt_*.c ../src/tooling/editor.c ../src/xorg_basalt.c ../src/assets_custom.dat.c -lX11 -lm -lXext -o editor_linux.x11 

cd ..

./build/editor_linux.x11
