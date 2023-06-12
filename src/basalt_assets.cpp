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
    fs::path assetFileName = fs::path(assetName).replace_filename(".ttf");
    for (auto& folder : AssetFolders) {
        auto combined = fs::path(folder) / assetFileName;
        if (fs::exists(combined)) {
            *outPath = combined.string();
            return true;
        }
    }
    SDL_LogError(0, "Asset not found: %s", assetName.c_str());
    return false;
}
