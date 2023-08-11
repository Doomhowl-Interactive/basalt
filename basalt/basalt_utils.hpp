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

void ReplaceAll(std::string& str, const std::string& from, const std::string& to);

std::string ConcatStrings(const std::vector<std::string> lines, char sep = '\n');

template <class T> inline void CombineHash(size_t& seed, const T& v)
{
    std::hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}
