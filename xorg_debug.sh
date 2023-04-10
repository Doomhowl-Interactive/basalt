#!/bin/sh
set -xe

sh xorg_run.sh -?
gdb -ex run --args ./build/basalt_linux.x11 --all
