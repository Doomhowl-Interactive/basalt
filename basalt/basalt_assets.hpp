#pragma once
#include <string>
#include <optional>
#include "basalt_graphics.hpp"

std::string& GetWorkingDirectory();
std::string& GetExecutableDirectory();

std::optional<std::string> SearchAsset(std::string assetName, std::string extension = "");
Texture LoadTexture(std::string name);