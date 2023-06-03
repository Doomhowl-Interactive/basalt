#!/bin/sh
set -xe
cmake -DCMAKE_BUILD_TYPE=Release -S . -B build
cmake --build build

mkdir -p ./build/dist
cp ./build/src/basalt ./build/dist
cp ./build/src/example/libexample_game.so ./build/dist