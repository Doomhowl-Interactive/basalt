project(example_game)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES *.cpp *.hpp)
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

set(LIBS
        basalt
        renderer_sdl_cpu
        SDL3-static
        SDL3_ttf-static
        )

target_link_libraries(${PROJECT_NAME} ${LIBS} ${PLAT_LIBS})

SET(EXTRA_HEADER_FOLDERS
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../vendor"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../basalt"
        )

target_include_directories(${PROJECT_NAME} PUBLIC ${EXTRA_HEADER_FOLDERS})
