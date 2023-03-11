@ECHO OFF

IF NOT EXIST build (
    MKDIR build
)

PUSHD build

DEL /F tools_embedder.exe
CL /ZI /FC /DDEBUG ..\src\tools_embedder.c
IF EXIST tools_embedder.exe (
    CALL tools_embedder.exe ..\assets ..\src\assets_custom.dat.c

    DEL /F basalt_win32.exe
    DEL /F basalt_win32_rel.exe

    rem CL /nologo /ZI /FC /EHa /Fe basalt_win32 /DDEBUG ..\src\basalt_*.c ..\src\assets_custom.dat.c -link -nodefaultlib -subsystem:windows user32.lib kernel32.lib
    CL /ZI /nologo /FC /EHa /Fe: basalt_win32.exe /DDEBUG /Tc ..\src\basalt_*.c /Tc ..\src\assets_custom.dat.c -link user32.lib kernel32.lib gdi32.lib

    IF EXIST basalt_win32.exe (
        ECHO Debug build completed!

        CL /nologo /O2 /Fe: basalt_win32_rel.exe /Tc ..\src\basalt_*.c /Tc ..\src\assets_custom.dat.c -link user32.lib kernel32.lib gdi32.lib

        IF EXIST basalt_win32_rel.exe (
            ECHO Release build completed!
        ) ELSE (
            ECHO Release build failed!
        )

    ) ELSE (
        ECHO Debug build failed!
    )
) ELSE (
    ECHO Failed to build tooling!
)

POPD

