@echo off

rem Some compilers don't like plain C for some reason
rem so the project should also be buildable with C++

pushd build
DEL /F basalt_win32.exe

rem Compile release build
CL /ZI /FC /NODEFAULTLIB /Fe basalt_win32 /TP ..\src\basalt_*.c /TP ..\src\assets_custom.dat.c user32.lib gdi32.lib

IF EXIST basalt_win32.exe (
    ECHO C++ Building completed!
) ELSE (
    ECHO C++ Building failed!
)
popd build
