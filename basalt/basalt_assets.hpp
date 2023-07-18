#pragma once
#include <string>
#include <optional>
#include <filesystem>
#include "basalt_graphics.hpp"

std::filesystem::path& GetWorkingDirectory();
std::filesystem::path& GetExecutableDirectory();

std::optional<std::string> SearchAsset(std::string assetName, std::string extension = "");
Texture LoadTexture(std::string name);