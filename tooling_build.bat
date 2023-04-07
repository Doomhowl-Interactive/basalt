@echo off
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
    set EMBEDDER=embedder.exe
    IF EXIST %EMBEDDER% (
        DEL /F %EMBEDDER%
    )

    CL /nologo /O2 /ZI /FC /Fe: %EMBEDDER% /I ..\src\tooling /Tp ..\src\tooling\embedder.c

    IF EXIST %EMBEDDER% (
        echo Built embedder

        call %EMBEDDER% ..\assets ..\src\assets_custom.dat.c
    )
POPD
