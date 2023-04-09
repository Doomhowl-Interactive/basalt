@echo off
set EXE_DEBUG=basalt_win32.exe
set EXE=basalt_win32_rel.exe

set TARGET_PLATFORM=x86

WHERE cl >nul 2>nul
IF %ERRORLEVEL% == 0 goto BUILD

IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat" (
  set VC_INIT="C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\VC\Auxiliary\Build\vcvarsall.bat"
) ELSE IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat" (
  set VC_INIT="C:\Program Files (x86)\Microsoft Visual Studio\2017\BuildTools\VC\Auxiliary\Build\vcvarsall.bat"
) ELSE IF EXIST "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat" (
  set VC_INIT="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
) ELSE IF EXIST "C:\Program Files (x86)\Microsoft Visual C++ Build Tools\vcbuildtools.bat" (
  set VC_INIT="C:\Program Files (x86)\Microsoft Visual C++ Build Tools\vcbuildtools.bat"
) ELSE IF EXIST "C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools\vsvars32.bat" (
  rem WINDOWS XP COMPILER
  set VC_INIT="C:\Program Files (x86)\Microsoft Visual Studio 10.0\Common7\Tools\vsvars32.bat"
  call %VC_INIT% > NUL 2>&1
  goto BUILD
) ELSE (
  echo "Couldn't find vcvarsall.bat or vcbuildtools.bat, please set it manually."
  exit /B 1
)

call %VC_INIT% %TARGET_PLATFORM% > NUL 2>&1

:BUILD

IF NOT EXIST build (
    MKDIR build
)

call tooling_build.bat

PUSHD build

    rem debug build
    CL /DEBUG /nologo /GS- /ZI /FC /EHa /Fe: %EXE_DEBUG% /Tc ..\src\basalt_*.c /Tc ..\src\bullet_game.c /Tc ..\src\win32_basalt.c /Tc ..\src\assets_custom.dat.c -link user32.lib gdi32.lib
    if %errorlevel% neq 0 popd; exit /b %errorlevel%

    IF EXIST %EXE_DEBUG% (
        ECHO Debug build completed!

        CL /nologo /O2 /Fe: %EXE% /Tc ..\src\basalt_*.c /Tc ..\src\win32_basalt.c /Tc ..\src\bullet_game.c /Tc ..\src\assets_custom.dat.c -link user32.lib gdi32.lib
        if %errorlevel% neq 0 popd; exit /b %errorlevel%

    ) 

POPD
