@echo off
DEL /F basalt_win32.exe
CL -ZI -FC "..\src\basalt_win32.c" "..\src\basalt_utils.c" "..\src\assets_custom.dat.c" user32.lib gdi32.lib

IF EXIST basalt_win32.exe (
    ECHO Building completed!
) ELSE (
    ECHO Building failed!
)
