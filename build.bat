@echo off

IF NOT EXIST build (
    mkdir build
)

pushd build
    cl -Zi ..\src\win32_basalt.c
popd
