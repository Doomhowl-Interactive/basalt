#include <sstream>
#include <vector>
#include <string>
#include <filesystem>

#include "basalt.h"
#include "basalt_internal.hpp"

namespace fs = std::filesystem;

static std::vector<std::string> AssetFolders = {
    ".", "assets", "../assets", "../../assets", "../../../assets",
};

INTERNAL bool SearchAsset(std::string assetName, std::string* outPath)
{
    fs::path assetFileName = fs::path(assetName).replace_extension(".ttf");
    std::vector<fs::path> traversedPaths;

    for (auto& folder : AssetFolders) {
        auto combined = fs::path(folder) / assetFileName;
        if (fs::exists(combined)) {
            *outPath = combined.string();
            return true;
        }
        traversedPaths.push_back(combined);
    }

    // Not found!: List all the places we looked for the asset file
    std::string msg = "Asset not found: " + assetName + "\n\nLooked in the following places:\n";
    for (auto& trav : traversedPaths) {
        msg += trav.string() + "\n";
    }
    SDL_LogError(0, "%s\n", msg.c_str());

    return false;
}