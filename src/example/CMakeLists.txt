cmake_minimum_required(VERSION 3.10)
project(example_game)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SRC_EXAMPLE example*.c example*.cpp example*.h example*.hpp)

if (WIN32)
    add_executable(${PROJECT_NAME} WIN32 ${SRC_EXAMPLE})
else()
    add_executable(${PROJECT_NAME} ${SRC_EXAMPLE})
endif()

# Include directories
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/..)

target_link_libraries(${PROJECT_NAME} basalt)
