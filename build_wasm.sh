#!/bin/sh
set -xe

echo "Building WASM..."
mkdir -p build
cd build

# build embedder
gcc ../src/tooling/embedder.c -o embedder
./embedder ../assets ../src/assets_custom.dat.c

clang  --target=wasm32 --no-standard-libraries -Wl,--no-entry                                      \
                                                           -Wl,--export=InitWASM                   \
                                                           -Wl,--export=GetWASMCanvasWidth         \
                                                           -Wl,--export=GetWASMCanvasHeight        \
                                                           -Wl,--export=GetWASMCanvas              \
                                                           -Wl,--export=PollWASMMousePosition      \
                                                           -Wl,--export=UpdateAndRenderWASM        \
                                                           -Wl,--export=LifeAndTheUniverse         \
      -fno-builtin -Wl,--allow-undefined -O2 -s -o basalt_wasm.wasm ../src/wasm_basalt.c           \
                                                                    ../src/basalt_*.c              \
                                                                    ../src/assets_custom.dat.c     \

cd ..
