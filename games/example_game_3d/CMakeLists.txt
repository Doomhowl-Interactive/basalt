project(example_game_3d)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

file(GLOB_RECURSE SOURCES *.cpp *.hpp)
add_executable(${PROJECT_NAME} WIN32 ${SOURCES})

set(LIBS
        basalt
        renderer_sdl_cpu
        SDL2-static
        SDL2_ttf
)

if (WIN32)
    set(PLAT_LIBS

    )
endif ()

target_link_libraries(${PROJECT_NAME} ${LIBS} ${PLAT_LIBS})

SET(EXTRA_HEADER_FOLDERS
        "${CMAKE_CURRENT_SOURCE_DIR}/../../vendor"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../basalt"
)

target_include_directories(${PROJECT_NAME} PUBLIC ${EXTRA_HEADER_FOLDERS})