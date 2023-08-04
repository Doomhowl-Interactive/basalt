#include "basalt_persistence.hpp"

#include <string>
#include <filesystem>
#include <spdlog/spdlog.h>

using namespace std;
namespace fs = std::filesystem;

bool CreateDirectories(const std::filesystem::path& path)
{
    if (fs::exists(path))
        return true;

    try {
        fs::create_directories(path);
        return true;
    } catch (const exception& e) {
        spdlog::error("Failed to create directories: {}", e.what());
        return false;
    }
}

optional<fs::path> TouchAppDataPath(string company, string appName)
{
#if defined(linux) || defined(__APPLE__)
    const auto configFolder = fs::path(getenv("HOME")) / ".local" / "config";
    if (!fs::exists(configFolder)) {
        spdlog::warn("The Linux share folder does not exist: {}. We are too uncertain about your "
                     "configuration. Use a sane distro like Debian or something. Using "
                     "the working directory.",
                     configFolder.string());
        return fs::current_path();
    }
    auto appFolder = configFolder / company / appName;
    CreateDirectories(appFolder);
    return appFolder;
#elif defined(_WIN32)
    auto localLowPath = fs::path(getenv("APPDATA")) / ".." / "LocalLow";
    if (!fs::exists(localLowPath)) {
        spdlog::warn("No LocalLow folder found on your Windows installation. Using the working directory...");
        return fs::current_path();
    }
    auto appFolder = localLowPath / company / appName;
    CreateDirectories(appFolder);
    return appFolder;
#else
    spdlog::warn("Running on unknown operating system, defaulting to working directory...");
    return fs::current_path();
#endif
}
