#!/bin/sh
set -xe

echo "Building WASM..."
mkdir -p build
cd build

# build embedder
gcc ../src/tooling/embedder.c -o embedder
./embedder ../assets ../src/assets_custom.dat.c

clang -fno-builtin --target=wasm32 --no-standard-libraries -Wl,--no-entry \
    -O2 -s -o wasm_basalt.wasm ../src/wasm_basalt.c

cd ..
