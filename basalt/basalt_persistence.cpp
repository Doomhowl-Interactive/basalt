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

optional<fs::path> TouchAppDataPath(string company, string appName) {
    auto path = GetAppDataPath(company, appName);
    if (path) {
        if (CreateDirectories(path.value())) {
            return path;
        }
    }
    return nullopt;
}

optional<fs::path> GetAppDataPath(string company, string appName)
{
#if defined(linux)
    const auto configFolder = fs::path(getenv("HOME")) / ".local" / "config";
    auto appFolder = configFolder / company / appName;
    return appFolder;
#elif defined(_WIN32)
    auto localLowPath = fs::path(getenv("APPDATA")) / ".." / "LocalLow";
    return localLowPath / company / appName;
#elif defined(__APPLE__)
    auto appSupport = fs::path(getenv("HOME")) / "Library" / "Application Support";
    return appSupport / company / appName;
#else
    spdlog::warn("Running on an unknown operating system, defaulting to working directory...");
    return fs::current_path();
#endif
}
