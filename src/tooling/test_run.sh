#!/bin/sh
gdc -frelease embedder_locale.d -o locale && \
    ./locale -i ~/dev/basalt/assets \
    -o ~/dev/basalt/src/locale_custom.dat.c -v