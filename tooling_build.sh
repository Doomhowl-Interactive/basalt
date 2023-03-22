#!/bin/sh

echo "Building..."
mkdir -p build
cd build

# build embedder
gcc ../src/tooling/embedder.c -o embedder && \
./embedder ../assets ../src/assets_custom.dat.c

cd ..
