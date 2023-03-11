@ECHO OFF

IF NOT EXIST build (
    MKDIR build
)

PUSHD build

rem DEL /F tools_embedder.exe
rem CL /ZI /FC /DDEBUG ..\src\tools_embedder.c
rem IF EXIST tools_embedder.exe (
rem     CALL tools_embedder.exe ..\assets ..\src\assets_custom.dat.c
    DEL /F basalt_win32.exe
    DEL /F basalt_win32_rel.exe

    CL /nologo /GS- /ZI /FC /EHa /Fe: basalt_win32.exe /DDEBUG /Tc ..\src\basalt_*.c /Tc ..\src\assets_custom.dat.c -link -nodefaultlib -subsystem:windows user32.lib kernel32.lib gdi32.lib

    IF EXIST basalt_win32.exe (
        ECHO Debug build completed!

        CL /nologo /GS- /Ot /Oy /Ob2 /GF /Gy /Fe: basalt_win32_rel.exe /Tc ..\src\basalt_*.c /Tc ..\src\assets_custom.dat.c -link -nodefaultlib -subsystem:windows user32.lib kernel32.lib gdi32.lib

        IF EXIST basalt_win32_rel.exe (
            ECHO Release build completed!
        ) ELSE (
            ECHO Release build failed!
        )

    ) ELSE (
        ECHO Debug build failed!
    )
rem ) ELSE (
rem     ECHO Failed to build tooling!
rem )

POPD

