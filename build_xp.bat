@echo off

rem We need to use the TP flag as Visual Studio 2010 fails to compile simple C for some reason?
pushd build
DEL /F basalt_win32.exe
CL /ZI /FC /TP "..\src\basalt_win32.c" "..\src\basalt_utils.c" "..\src\assets_custom.dat.c"

IF EXIST basalt_win32.exe (
    ECHO Building completed!
) ELSE (
    ECHO Building failed!
)
popd build
