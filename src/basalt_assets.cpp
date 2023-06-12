#include <vector>
#include <string>
#include <filesystem>

#include "basalt_internal.hpp"

namespace fs = std::filesystem;

static std::vector<std::string> AssetFolders = {
    ".", "assets", "../assets", "../../assets", "../../../assets",
};

INTERNAL std::string SearchAsset(std::string assetName)
{
    fs::path assetFileName = fs::path(assetName).replace_filename(".ttf");
    for (auto& folder : AssetFolders) {
        auto combined = fs::path(folder) / assetFileName;
        if (fs::exists(combined)) {
            return combined;
        }
    }
    throw new std::runtime_error("Asset not found: " + assetName);
}

