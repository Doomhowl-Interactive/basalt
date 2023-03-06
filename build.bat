@echo off

IF NOT EXIST build (
    mkdir build
)

pushd build
    cl -Zi -FC ..\src\win32_basalt.c user32.lib gdi32.lib
popd
