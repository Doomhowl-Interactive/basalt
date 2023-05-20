project(basalt)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SRC_BASALT basalt_*.c *basalt_*.h)
file(GLOB_RECURSE SRC_BULLET bullet_*.c *bullet_*.h)

set (LIBS -lm)

if (WIN32)
    file(GLOB_RECURSE SRC_PLAT win32_*.c)
else ()
    file(GLOB_RECURSE SRC_PLAT xorg_*.c)
    set(LIBS ${LIBS} -lX11 -lXext)
endif ()

set(SRC_ALL ${SRC_BASALT} ${SRC_BULLET} ${SRC_PLAT})

add_compile_options(-ggdb -Wall)
link_libraries(${LIBS})

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SRC_ALL})
else()
    add_executable(${PROJECT_NAME} ${SRC_ALL})
endif()
