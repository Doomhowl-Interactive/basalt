#pragma once
#include <string>
#include <optional>
#include <filesystem>

long GetFileModifiedTime(const std::string filePath);
std::string GetFileName(const std::string filePath);
std::string GetFileStem(const std::string filePath);

// NOTE: Returns NULL when none exist,
// pass NULL ended array
std::optional<std::string> GetFirstExistingFolder(const std::vector<std::string> folders);
std::vector<std::string> GetFolderFiles(const std::string folder, const std::string ext);
