project(basalt_3d)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES *.cpp *.hpp *.h)
add_library(${PROJECT_NAME} STATIC ${SOURCES})

SET(EXTRA_HEADER_FOLDERS
        "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/SDL/include/SDL3"
        "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/SDL/include"
        "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/SDL_ttf/include/SDL3"
        "${CMAKE_CURRENT_SOURCE_DIR}/../vendor"
)

target_include_directories(${PROJECT_NAME} PUBLIC ${EXTRA_HEADER_FOLDERS})