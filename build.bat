@echo off

IF NOT EXIST build (
    mkdir build
)

pushd build
    del /F ..\src\tools_embedder.exe
    cl -Zi -FC ..\src\tools_embedder.c
    del /F ..\src\assets_custom.dat.c
    call tools_embedder.exe
    cl -Zi -FC ..\src\win32_basalt.c ..\src\assets_custom.dat.c user32.lib gdi32.lib
popd
