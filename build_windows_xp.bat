@echo off
set EXE=basalt_winxp_rel.exe

WHERE cl >nul 2>nul
IF %ERRORLEVEL% == 0 goto BUILD

IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools\vsvars32.bat" (
  set VC_INIT="C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools\vsvars32.bat"
  goto BUILD
) ELSE (
  echo "Couldn't find vsvars32.bat, please set it manually."
  exit /B
)

call %VC_INIT% > NUL 2>&1

:BUILD

rem Some compilers don't like plain C for some reason
rem so the project should buildable with C++ also

IF NOT EXIST build (
    MKDIR build
)

pushd build

    IF EXIST %EXE% (
        DEL /F %EXE%
    )

    rem Compile release build
    CL /nologo /GS- /Ot /Oy /Ob2 /GF /Gy /Fe: %EXE% /TP ..\src\basalt_*.c /TP ..\src\win32_basalt.c /TP ..\src\assets_custom.dat.c -link -nodefaultlib -subsystem:windows kernel32.lib user32.lib gdi32.lib

    IF EXIST %EXE% (
        ECHO C++ XP Building completed!
    ) ELSE (
        ECHO C++ XP Building failed!
    )

popd
