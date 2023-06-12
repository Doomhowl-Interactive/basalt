project(basalt)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Find source files
file(GLOB_RECURSE SRC_BASALT basalt_*.c basalt_*.cpp basalt_*.h basalt_*.hpp)
if (UNIX OR LINUX)
    file(GLOB_RECURSE SRC_PLAT unix_*.c unix_*.cpp)
elseif (WIN32)
    file(GLOB_RECURSE SRC_PLAT windows_*.c windows_*.cpp)
endif ()

# Compile cross-platform SDL2
file(GLOB_RECURSE SRC_SDL2 sdl2_*.c sdl2_*.cpp sdl2_*.h sdl2_*.hpp)

add_compile_options(-ggdb -Wall)

set(SRC_ALL ${SRC_BASALT} ${SRC_PLAT} ${SRC_SDL2})
add_library(${PROJECT_NAME} STATIC ${SRC_ALL})

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Import packages
target_link_libraries(${PROJECT_NAME} m)

find_package(SDL2 REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::Main)

find_package(SDL2_ttf REQUIRED)
target_link_libraries(${PROJECT_NAME} SDL2::TTF)
