@ECHO OFF

IF NOT EXIST build (
    MKDIR build
)

PUSHD build

DEL /F tools_embedder.exe
REM CL -ZI -FC /O2 "..\src\tools_embedder.c"
CL -ZI -FC /DDEBUG "..\src\tools_embedder.c"
IF EXIST tools_embedder.exe (
    CALL tools_embedder.exe "..\assets" "..\src\assets_custom.dat.c"

    DEL /F win32_basalt.exe
    CL -ZI -FC "..\src\win32_basalt.c" "..\src\assets_custom.dat.c" user32.lib gdi32.lib

    IF EXIST win32_basalt.exe (
        ECHO Building completed!
    ) ELSE (
        ECHO Building failed!
    )
) ELSE (
    ECHO Failed to build tooling!
)

POPD

