project(basalt)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(SDL2_DIR ../vendor/SDL2-2.26.5/cmake)
    set(SDL2_DIR_ARCH ../vendor/SDL2-2.26.5/x86_64-w64-mingw32)
endif ()

file(GLOB_RECURSE SRC_BASALT basalt*.c basalt*.cpp *basalt*.h*)
file(GLOB_RECURSE SRC_EXAMPLE example*.c example*.cpp example*.h)

find_package(SDL2 REQUIRED)

if (UNIX OR LINUX)
    file(GLOB_RECURSE SRC_PLAT unix_*.c unix_*.cpp)
elseif (WIN32)
    file(GLOB_RECURSE SRC_PLAT windows_*.c windows_*.cpp)
endif ()

# Compile cross-platform SDL2
file(GLOB_RECURSE SRC_SDL2 sdl2_*.c sdl2_*.cpp sdl2_*.h*)

set(SRC_ALL ${SRC_BASALT} ${SRC_PLAT} ${SRC_SDL2} ${SRC_EXAMPLE})

add_compile_options(-ggdb -Wall)
link_libraries(-lm ${SDL2_LIBRARIES})

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SRC_ALL})
    target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_DIR_ARCH}/include/)
    file(COPY ${SDL2_DIR_ARCH}/bin/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else()
    add_executable(${PROJECT_NAME} ${SRC_ALL})
endif()
