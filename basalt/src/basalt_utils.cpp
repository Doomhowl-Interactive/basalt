#include <iostream>
#include <cassert>
#include <cstdarg>
#include <windows.h>
#include <filesystem>
#include <iostream>

#include "basalt_math.hpp"
#include "basalt_utils.hpp"
#include "basalt_console.hpp"

namespace basalt {

using namespace std;

namespace fs = std::filesystem;

string& GetWorkingDirectory()
{
    static string workingDirectory = "";
    if (workingDirectory == "") {
        char buffer[MAX_PATH];
        GetCurrentDirectoryA(MAX_PATH, buffer);
        workingDirectory = buffer;
    }
    return workingDirectory;
}

string& GetExecutableDirectory()
{
    static string executableDirectory = "";
    if (executableDirectory == "") {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        executableDirectory = buffer;
        executableDirectory
            = executableDirectory.substr(0, executableDirectory.find_last_of("\\/"));
    }
    return executableDirectory;
}

// adapted from Raylib
// WARN: Cached memory, copy for long usage!
const char* FormatText(const char* text, ...)
{
    constexpr int MAX_TEXT_LEN = 1024;
    constexpr int TEXT_BUFFER_COUNT = 16;

    assert(text);

    static char buffers[TEXT_BUFFER_COUNT][MAX_TEXT_LEN];
    static size_t curBufferIndex = 0;

    char* currentBuffer = buffers[curBufferIndex++];
    curBufferIndex %= TEXT_BUFFER_COUNT;

    va_list args;
    va_start(args, text);
    vsnprintf(currentBuffer, MAX_TEXT_LEN, text, args);

    return currentBuffer;
}

string& TrimText(string buffer)
{
    buffer = buffer.substr(0, buffer.find_last_not_of(" \n\r\t") + 1);
    return buffer;
}

string& StripText(string buffer)
{
    buffer.erase(remove_if(buffer.begin(), buffer.end(), isspace), buffer.end());
    return buffer;
}

string& PadStringRight(string text, char symbol, size_t length)
{
    while (text.length() < length) {
        text += symbol;
    }
    return text;
}

string& PadStringLeft(string text, char symbol, size_t length)
{
    while (text.length() < length) {
        text = symbol + text;
    }
    return text;
}

bool FileHasExtension(const string name, const string ext)
{
    return fs::path(name).extension() == ext;
}

bool FolderExists(const string folder)
{
    if (fs::exists(folder)) {
        return fs::is_directory(folder);
    }
    return false;
}

long GetFileModifiedTime(const std::string filePath)
{
    if (fs::exists(filePath)) {
        auto ftime = fs::last_write_time(filePath);
        return ftime.time_since_epoch().count();
    }
    return 0;
}

string GetFileName(const string filePath)
{
    return fs::path(filePath).filename().string();
}

string GetFileStem(const string filePath)
{
    return fs::path(filePath).stem().string();
}

optional<const string&> GetFirstExistingFolder(const vector<string> folders)
{
    for (auto& folder : folders) {
        if (FolderExists(folder)) {
            return folder;
        }
    }
    return nullopt;
}

vector<string> GetFolderFiles(const string folder, const string ext)
{
    vector<string> files;

    // Iterate over the files in the specified folder
    for (const auto& entry : fs::directory_iterator(folder)) {
        // Check if the current entry is a regular file and has the specified extension
        if (entry.is_regular_file() && entry.path().extension() == ext) {
            // Add the file name to the vector
            files.push_back(entry.path().filename().string());
        }
    }

    return files;
}

}  // namespace basalt