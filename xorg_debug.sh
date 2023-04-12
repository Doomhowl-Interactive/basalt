#!/bin/sh
make && gdb -ex run --args ./build/basalt_linux.x11 --all
