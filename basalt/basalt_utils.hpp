#pragma once

#include <string>
#include <vector>
#include <cstdarg>
#include <optional>

// WARN: Cached memory, copy for long usage!
std::string FormatText(const char* text, ...);
std::string& TrimText(std::string buffer);
std::string& StripText(std::string buffer);
std::string& PadStringRight(std::string text, char symbol, size_t length);
std::string& PadStringLeft(std::string text, char symbol, size_t length);

bool FileHasExtension(const std::string folder, const std::string ext);
bool FolderExists(const std::string folder);

std::string ConcatStrings(const std::vector<std::string> lines, char sep = '\n');

template <class T> inline void CombineHash(size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// WARN: Uses cached memory, result gets overwritten on future calls.
// NOTE: Get seconds since epoch a file was last changed.
// Returns 0 if file doesn't exists.
long GetFileModifiedTime(const std::string filePath);
std::string GetFileName(const std::string filePath);
std::string GetFileStem(const std::string filePath);

// NOTE: Returns NULL when none exist,
// pass NULL ended array
std::optional<std::string> GetFirstExistingFolder(const std::vector<std::string> folders);
std::vector<std::string> GetFolderFiles(const std::string folder, const std::string ext);
