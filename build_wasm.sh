#!/bin/sh
set -xe

echo "Building WASM..."
mkdir -p build
cd build

# build embedder
gcc ../src/tooling/embedder.c -o embedder
./embedder ../assets ../src/assets_custom.dat.c

clang --target=wasm32 --no-standard-libraries -Wl,--no-entry -DWASM \
      -Wl,--export-all \
      -fno-builtin -Wl,--allow-undefined -O3 -s -Wl,-z,stack-size=134217728 -o basalt_wasm.wasm \
      ../src/wasm_basalt.c

cd ..
