@echo off

IF NOT EXIST build (
    MKDIR build
)

pushd build
DEL /F basalt_stdlib_tests.exe
CL /nologo /ZI /FC /GS- /DMAIN ..\src\basalt_stdlib_tests.c -link -nodefaultlib kernel32.lib

IF EXIST basalt_stdlib_tests.exe (
    call basalt_stdlib_tests.exe
    IF %ErrorLevel% == 0 (
        echo Stdlib test succeeded!
    ) ELSE (
        echo Stdlib test failed %ErrorLevel%!
    )

) ELSE (
    echo Build failed!
)

popd
