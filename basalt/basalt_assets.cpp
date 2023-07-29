#include <vector>
#include <string>
#include <filesystem>
#include <optional>
#include <cassert>
#include <spdlog/spdlog.h>

#include "basalt_macros.hpp"
#include "basalt_assets.hpp"
#include "basalt_console.hpp"
#include "basalt_images.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "external/stb_image.h"

namespace fs = std::filesystem;
using namespace std;

static void RelocateToExecutable()
{
    // Change the working directory to the executable folder.
    // This is needed because so the assets are always found.
    auto executablePath = GetExecutableDirectory();
    if (executablePath) {
        if (fs::current_path() != executablePath) {
            fs::current_path(executablePath.value());
            spdlog::debug("Relocated to executable folder: {}", executablePath.value().string());
        }
    } else {
        spdlog::error("Relocating is only possible on Windows!");
    }
}

optional<string> SearchAsset(string assetName, string extension)
{
    // HACK: !
    static const vector<string> AssetFolders
        = { "assets",
            "../assets",
            "../../assets",
            "../../../assets",
            "/Users/bram/dev/basalt/basalt/assets",
            "/Users/bram/dev/basalt/games/snake-feeder/assets" };

    RelocateToExecutable();

    fs::path assetFileName = fs::path(assetName);
    if (!extension.empty() && !assetFileName.has_extension()) {
        assetFileName = assetFileName.replace_extension(extension);
    }
    vector<fs::path> traversedPaths;

    for (auto& folder : AssetFolders) {
        auto comb_folder = GetWorkingDirectory() / folder;
        if (fs::exists(comb_folder)) {
            auto combined = comb_folder / assetFileName;
            if (fs::exists(combined)) {
                return combined.string();
            } else {
                spdlog::warn("Asset path {} does not exist!", combined.string());
            }
            traversedPaths.push_back(combined);
        } else {
            spdlog::warn("Asset path {} does not exist!", comb_folder.string());
        }
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

// TODO: clean this up into win32_basalt and unix_basalt
fs::path& GetWorkingDirectory()
{
    static fs::path workingDirectory = "";
#ifdef WIN32
    if (workingDirectory == "") {
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        workingDirectory = buffer;
    }
#elif defined(__linux__) || defined(__APPLE__)
    if (workingDirectory == "") {
        char buffer[PATH_MAX];
        getcwd(buffer, PATH_MAX);
        workingDirectory = buffer;
    }
#endif
    return workingDirectory;
}

optional<fs::path> GetExecutableDirectory()
{
    static fs::path executableDirectory = "";
#ifdef WIN32
    if (executableDirectory == "") {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        executableDirectory = buffer;
        executableDirectory = executableDirectory.parent_path();
    }
#endif
    if (executableDirectory == "") {
        return nullopt;
    } else {
        return executableDirectory;
    }
}

// TODO: Use SDL_Image to load the texture
static void LoadTextureFromStbData(Image& texture, uchar* data, int channels)
{
    assert(data);
    assert(texture.width > 0 && texture.height > 0);
    vector<Color> pixels(texture.width * texture.height);
    if (channels == 4 || channels == 3) {
        // Copy the texture into the correct color order
        auto* comps = (uchar*)pixels.data();
        for (int i = 0; i < texture.width * texture.height; i++) {
            comps[i * 4 + 0] = data[i * 4 + 3];
            comps[i * 4 + 1] = data[i * 4 + 2];
            comps[i * 4 + 2] = data[i * 4 + 1];
            comps[i * 4 + 3] = data[i * 4 + 0];
        }
    } else {
        spdlog::error("Unexpected amount of channels in image: there are {}!", channels);
    }
    texture.SetPixels(pixels);
}

// TODO: Support loading grayscale textures
Image LoadTexture(string name)
{
    auto asset = SearchAsset(name, "png");
    if (asset.has_value()) {
        int width, height;
        int channels = 0;

        const char* assetPath = asset.value().c_str();
        auto* data = (uchar*)stbi_load(assetPath, &width, &height, &channels, 4);

        spdlog::debug(
            "Loaded texture {} of size {}x{} with {} channels", name, width, height, channels);

        Image texture = Image(width, height);
        if (data) {
            LoadTextureFromStbData(texture, data, channels);
            stbi_image_free(data);
        } else {
            spdlog::error("Failed to load texture {} at {}", name, assetPath);
        }
        return texture;
    }
    Image error = Image(32, 32);
    error.Clear(0xFF00FFFF);
    return error;
}
