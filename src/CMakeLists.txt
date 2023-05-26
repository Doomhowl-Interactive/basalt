project(basalt)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SRC_BASALT basalt*.c *basalt*.h)
file(GLOB_RECURSE SRC_BULLET bullet*.c *bullet*.h)

set (LIBS -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer)

if (UNIX OR LINUX)
    file(GLOB_RECURSE SRC_PLAT unix_*.c)
elseif (WIN32)
    file(GLOB_RECURSE SRC_PLAT windows_*.c)
endif ()

# Compile cross-platform SDL2
file(GLOB_RECURSE SRC_SDL2 sdl2_*.c)

set(SRC_ALL ${SRC_BASALT} ${SRC_PLAT} ${SRC_SDL2} ${SRC_BULLET})

add_compile_options(-ggdb -Wall)
link_libraries(${LIBS})

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SRC_ALL})
else()
    add_executable(${PROJECT_NAME} ${SRC_ALL})
endif()
