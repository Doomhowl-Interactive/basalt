set(SDL3TTF_VENDORED ON)
set(BUILD_SHARED_LIBS OFF)

set(SDL_SHARED OFF)
set(SDL_STATIC ON)
set(SDL_TEST OFF)
set(SDL_TESTS OFF)

add_subdirectory(SDL)
add_subdirectory(SDL_ttf)