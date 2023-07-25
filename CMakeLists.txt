cmake_minimum_required(VERSION 3.10)
project(basalt)

set(SDL3TTF_VENDORED ON)

add_subdirectory(vendor/SDL)
add_subdirectory(vendor/SDL_ttf)
add_subdirectory(basalt)