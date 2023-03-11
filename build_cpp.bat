@echo off

rem Some compilers don't like plain C for some reason
rem so the project should also be buildable with C++

IF NOT EXIST build (
    MKDIR build
)

pushd build
DEL /F basalt_win32.exe

rem Compile release build
CL /nologo /GS- /Ot /Oy /Ob2 /GF /Gy /Fe: basalt_win32_rel_cpp /TP ..\src\basalt_*.c /TP ..\src\assets_custom.dat.c -link -nodefaultlib -subsystem:windows kernel32.lib user32.lib gdi32.lib

IF EXIST basalt_win32_rel_cpp.exe (
    ECHO C++ Building completed!
) ELSE (
    ECHO C++ Building failed!
)
popd build
