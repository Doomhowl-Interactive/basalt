project(basalt)
set(CMAKE_C_STANDARD 11)

if (WIN32)
    set(SDL2_DIR ../vendor/SDL2-2.26.5/cmake)
endif ()

file(GLOB_RECURSE SRC_BASALT basalt*.c *basalt*.h)
file(GLOB_RECURSE SRC_BULLET bullet*.c *bullet*.h)

find_package(SDL2 REQUIRED)

if (UNIX OR LINUX)
    file(GLOB_RECURSE SRC_PLAT unix_*.c)
elseif (WIN32)
    file(GLOB_RECURSE SRC_PLAT windows_*.c)
endif ()

# Compile cross-platform SDL2
file(GLOB_RECURSE SRC_SDL2 sdl2_*.c)

set(SRC_ALL ${SRC_BASALT} ${SRC_PLAT} ${SRC_SDL2} ${SRC_BULLET})

add_compile_options(-ggdb -Wall)
link_libraries(-lm ${SDL2_LIBRARIES})

if (WIN32)
    add_executable(${PROJECT_NAME} ${SRC_ALL})
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR}/../x86_64-w64-mingw32/include/)
else()
    add_executable(${PROJECT_NAME} ${SRC_ALL})
endif()
