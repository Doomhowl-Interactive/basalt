@echo off

rem Some compilers don't like plain C for some reason
rem so the project should also be buildable with C++

pushd build
DEL /F basalt_win32.exe
CL /ZI /FC /Tp ..\src\basalt_win32.c /Tp ..\src\basalt_utils.c /Tp ..\src\assets_custom.dat.c user32.lib gdi32.lib

IF EXIST basalt_win32.exe (
    ECHO C++ Building completed!
) ELSE (
    ECHO C++ Building failed!
)
popd build
