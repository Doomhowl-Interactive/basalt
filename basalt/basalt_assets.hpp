#pragma once
#include <string>
#include <optional>
#include <filesystem>
#include "basalt_images.hpp"

std::filesystem::path& GetWorkingDirectory();
std::optional<std::string> SearchAsset(std::string assetName, std::string extension = "");
Image LoadTexture(std::string name);
