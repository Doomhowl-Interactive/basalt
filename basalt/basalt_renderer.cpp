#pragma once
#include <unordered_map>

#include "basalt_textures.hpp"

using namespace std;

static unordered_map<size_t, Texture> textures;

size_t GetNextTextureID()
{
    static size_t id = 0;
    return id++;
}