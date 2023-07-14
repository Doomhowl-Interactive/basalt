#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <optional>

#include "basalt_assets.hpp"
#include "basalt_console.hpp"
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;
using namespace std;

static vector<string> AssetFolders = {
    "assets",
    "../assets",
    "../../assets",
    "../../../assets",
};

optional<string> SearchAsset(string assetName)
{
    fs::path assetFileName = fs::path(assetName).replace_extension(".ttf");
    vector<fs::path> traversedPaths;

    for (auto& folder : AssetFolders) {
        auto combined = fs::path(folder) / assetFileName;
        if (fs::exists(combined)) {
            return combined.string();
        }
        traversedPaths.push_back(combined);
    }

    // Not found!: List all the places we looked for the asset file
    string msg = "Asset not found: " + assetName + "\n\nLooked in the following places:\n";
    for (auto& trav : traversedPaths) {
        msg += trav.string() + "\n";
    }
    spdlog::error(msg);

    return nullopt;
}
