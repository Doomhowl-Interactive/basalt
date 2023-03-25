#!/bin/sh

echo "Building..."
mkdir -p build
cd build

# build embedder
gcc -ggdb ../src/tooling/embedder.c -o embedder && \
./embedder ../assets ../src/assets_custom.dat.c

# build archaeo
gcc -ggdb ../src/tooling/archaeo.c -o archaeo

cd ..
