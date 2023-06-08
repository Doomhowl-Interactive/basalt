project(basalt)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (WIN32)
    set(SDL2_DIR ${CMAKE_SOURCE_DIR}/vendor/SDL2-2.26.5/cmake)
    set(SDL2_DIR_ARCH ${CMAKE_SOURCE_DIR}/vendor/SDL2-2.26.5/x86_64-w64-mingw32)
	# Hack for basalt being used as a sub-project
	if (NOT EXISTS ${SDL2_DIR_ARCH})
		set(SDL2_DIR_ARCH ${CMAKE_SOURCE_DIR}/vendor/basalt/vendor/SDL2-2.26.5/x86_64-w64-mingw32)
	endif()
	set(SDL2_DIR ${SDL2_DIR_ARCH}/../cmake)
    message(STATUS "Using SDL2 library at: ${SDL2_DIR_ARCH}")
endif ()

file(GLOB_RECURSE SRC_BASALT basalt*.c basalt*.cpp *basalt*.h*)

find_package(SDL2 REQUIRED)

if (UNIX OR LINUX)
    file(GLOB_RECURSE SRC_PLAT unix_*.c unix_*.cpp)
elseif (WIN32)
    file(GLOB_RECURSE SRC_PLAT windows_*.c windows_*.cpp)
endif ()

# Compile cross-platform SDL2
file(GLOB_RECURSE SRC_SDL2 sdl2_*.c sdl2_*.cpp sdl2_*.h*)

set(SRC_ALL ${SRC_BASALT} ${SRC_PLAT} ${SRC_SDL2})

add_compile_options(-ggdb -Wall)
link_libraries(-lm ${SDL2_LIBRARIES})

add_library(${PROJECT_NAME} STATIC ${SRC_ALL})
