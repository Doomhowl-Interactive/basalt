#include <iostream>
#include <cassert>
#include <cstdarg>
#include <filesystem>
#include <iostream>
#include <optional>
#include <vector>

#include "basalt_math.hpp"
#include "basalt_utils.hpp"
#include "basalt_console.hpp"

using namespace std;

namespace fs = std::filesystem;

// adapted from Raylib
string FormatText(const char* text, ...)
{
    constexpr int MAX_TEXT_LEN = 1024;
    constexpr int TEXT_BUFFER_COUNT = 16;

    assert(text);

    char buffer[MAX_TEXT_LEN];

    va_list args;
    va_start(args, text);
    vsnprintf(buffer, MAX_TEXT_LEN, text, args);

    return buffer;
}

string& TrimText(string buffer)
{
    buffer = buffer.substr(0, buffer.find_last_not_of(" \n\r\t") + 1);
    return buffer;
}

string& StripText(string buffer)
{
    string result = "";
    for (char c : buffer) {
        if (c != ' ' && c != '\n' && c != '\r' && c != '\t') {
            result += c;
        }
    }
    return result;
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

string ConcatStrings(const vector<string> lines, char sep)
{
    string result;
    for (const auto& line : lines) {
        result += line + "\n";
    }
    result.pop_back();
    return result;
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

optional<string> GetFirstExistingFolder(const vector<string> folders)
{
    for (const auto& folder : folders) {
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
