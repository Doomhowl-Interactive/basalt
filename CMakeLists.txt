cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_COMPILER clang)
set(CMAKE_C_COMPILER clang)

project(basalt)

add_subdirectory(vendor)
add_subdirectory(basalt)
add_subdirectory(renderers)

add_subdirectory(games)