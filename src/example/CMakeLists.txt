cmake_minimum_required(VERSION 3.10)
project(example_game)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SRC_EXAMPLE example*.c example*.cpp example*.h*)

link_libraries(basalt)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SRC_EXAMPLE})
    set(SDL2_DIR_ARCH ${CMAKE_SOURCE_DIR}/vendor/SDL2-2.26.5/x86_64-w64-mingw32)
    file(COPY ${SDL2_DIR_ARCH}/bin/SDL2.dll DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
else()
    add_executable(${PROJECT_NAME} ${SRC_EXAMPLE})
endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)