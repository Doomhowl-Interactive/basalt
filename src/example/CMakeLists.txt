cmake_minimum_required(VERSION 3.10)
project(example_game)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC_EXAMPLE example*.c example*.cpp example*.h*)

add_library(${PROJECT_NAME} SHARED ${SRC_EXAMPLE})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/..)