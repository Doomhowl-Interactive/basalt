#pragma once
#include <filesystem>
#include <optional>

#include "basalt_macros.hpp"

// Aims to be the equivalent to with the Doomhowl tauri web-based games.
std::optional<std::filesystem::path> GetAppDataPath(std::string company, std::string appName);
std::optional<std::filesystem::path> TouchAppDataPath(std::string company, std::string appName);

static bool CreateDirectories(const std::filesystem::path& path);
