#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <optional>
#include <cassert>
#include <spdlog/spdlog.h>

#include "basalt_macros.hpp"
#include "basalt_assets.hpp"
#include "basalt_console.hpp"
#include "basalt_graphics.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

namespace fs = std::filesystem;
using namespace std;

static void RelocateToExecutable()
{
    // Change the working directory to the executable folder.
    // This is needed because so the assets are always found.
    fs::path executablePath = GetExecutableDirectory();
    if (fs::current_path() != executablePath) {
        fs::current_path(executablePath);
        spdlog::debug("Relocated to executable folder: {}", executablePath.string());
    }
}

optional<string> SearchAsset(string assetName, string extension)
{
    static const vector<string> AssetFolders = {
        "assets",
        "../assets",
        "../../assets",
        "../../../assets",
    };

    RelocateToExecutable();

    fs::path assetFileName = fs::path(assetName);
    if (!extension.empty() && !assetFileName.has_extension()) {
        assetFileName = assetFileName.replace_extension(extension);
    }
    vector<fs::path> traversedPaths;

    for (auto& folder : AssetFolders) {
        auto combined = fs::path(folder) / assetFileName;
        if (fs::exists(combined)) {
            return combined.string();
        }
        traversedPaths.push_back(combined);
    }
    assert(AssetFolders.size() > 0);

    // Not found!: List all the places we looked for the asset file
    string msg = "Asset not found: " + assetName + " (assumed " + assetFileName.string()
                 + ")\n\nLooked in the following places:\n";

    for (auto& trav : traversedPaths) {
        msg += trav.string() + "\n";
    }

    msg += "\nThe program is ran from: " + fs::current_path().string();
    spdlog::error(msg);

    return nullopt;
}

fs::path& GetWorkingDirectory()
{
    static fs::path workingDirectory = "";
    if (workingDirectory == "") {
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        workingDirectory = buffer;
    }
    return workingDirectory;
}

fs::path& GetExecutableDirectory()
{
    static fs::path executableDirectory = "";
    if (executableDirectory == "") {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        executableDirectory = buffer;
        executableDirectory = executableDirectory.parent_path();
    }
    return executableDirectory;
}

static void LoadTextureFromStbData(Texture texture, uchar* data, int channels)
{
    assert(data);
    assert(texture.width > 0 && texture.height > 0);

    if (channels == 4 || channels == 3) {
        // Copy the texture into the correct color order
        uchar* comps = (uchar*)texture.pixels.get()->data();
        for (int i = 0; i < texture.width * texture.height; i++) {
            comps[i * 4 + 0] = data[i * 4 + 3];
            comps[i * 4 + 1] = data[i * 4 + 2];
            comps[i * 4 + 2] = data[i * 4 + 1];
            comps[i * 4 + 3] = data[i * 4 + 0];
        }
    } else {
        spdlog::error("Unexpected amount of channels in image: there are {}!", channels);
    }
}

// TODO: Support loading grayscale textures
Texture LoadTexture(string name)
{
    auto asset = SearchAsset(name, "png");
    if (asset.has_value()) {
        int width, height;
        int channels = 0;

        const char* assetPath = asset.value().c_str();
        uchar* data = (uchar*)stbi_load(assetPath, &width, &height, &channels, 4);

        spdlog::debug(
            "Loaded texture {} of size {}x{} with {} channels", name, width, height, channels);

        Texture texture = Texture(width, height);
        texture.name = name;

        if (data) {
            LoadTextureFromStbData(texture, data, channels);
            stbi_image_free(data);
        } else {
            spdlog::error("Failed to load texture {} at {}", name, assetPath);
        }
        return texture;
    }
    Texture error = Texture(32, 32);
    error.Clear(0xFF00FFFF);
    return error;
}