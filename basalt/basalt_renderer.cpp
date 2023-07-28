#pragma once
#include <unordered_map>

#include "basalt_images.hpp"

using namespace std;

static unordered_map<size_t, Image> textures;

size_t GetNextTextureID()
{
    static size_t id = 0;
    return id++;
}