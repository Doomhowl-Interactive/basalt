@ECHO OFF

IF NOT EXIST build (
    MKDIR build
)

PUSHD build

DEL /F tools_embedder.exe
CL /ZI /FC ..\src\tools_embedder.c
IF EXIST tools_embedder.exe (
    CALL tools_embedder.exe ..\assets ..\src\assets_custom.dat.c

    DEL /F basalt_win32.exe
    CL /ZI /FC ..\src\basalt_win32.c ..\src\basalt_utils.c ..\src\assets_custom.dat.c user32.lib gdi32.lib

    IF EXIST basalt_win32.exe (
        ECHO Building completed!
    ) ELSE (
        ECHO Building failed!
    )
) ELSE (
    ECHO Failed to build tooling!
)

POPD

