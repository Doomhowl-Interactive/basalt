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
  exit /B
)

call %VC_INIT% %TARGET_PLATFORM% > NUL 2>&1

:BUILD

IF NOT EXIST build (
    MKDIR build
)

PUSHD build

    IF EXIST %EXE% (
        DEL /F %EXE%
    )
    IF EXIST %EXE_DEBUG% (
        DEL /F %EXE_DEBUG%
    )

    rem debug build
    CL /nologo /GS- /ZI /FC /EHa /Fe: %EXE_DEBUG% /DDEBUG /Tc ..\src\basalt_*.c /Tc ..\src\assets_custom.dat.c -link -nodefaultlib -subsystem:windows user32.lib kernel32.lib gdi32.lib

    IF EXIST %EXE_DEBUG% (
        ECHO Debug build completed!

        CL /nologo /GS- /Ot /Oy /Ob2 /GF /Gy /Fe: %EXE% /Tc ..\src\basalt_*.c /Tc ..\src\assets_custom.dat.c -link -nodefaultlib -subsystem:windows user32.lib kernel32.lib gdi32.lib

        IF EXIST %EXE% (
            ECHO Release build completed!
        ) ELSE (
            ECHO Release build failed!
        )

    ) ELSE (
        ECHO Debug build failed!
    )

POPD
