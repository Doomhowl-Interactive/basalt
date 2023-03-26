#!/bin/bash
set -xe

mkdir -p build

cd build

cc ../temple_src/temple_game.c -o templegame.x11 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -I ../src

cd ..
