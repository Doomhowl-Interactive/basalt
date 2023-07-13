#pragma once

#include <vector>
#include <cstdarg>
#include <optional>

namespace basalt {

using namespace std;

string& GetWorkingDirectory();
string& GetExecutableDirectory();

// WARN: Cached memory, copy for long usage!
string FormatText(const char* text, ...);
string& TrimText(string buffer);
string& StripText(string buffer);
string& PadStringRight(string text, char symbol, size_t length);
string& PadStringLeft(string text, char symbol, size_t length);

bool FileHasExtension(const string folder, const string ext);
bool FolderExists(const string folder);

// WARN: Uses cached memory, result gets overwritten on future calls.
// NOTE: Get seconds since epoch a file was last changed.
// Returns 0 if file doesn't exists.
long GetFileModifiedTime(const string filePath);
string GetFileName(const string filePath);
string GetFileStem(const string filePath);

// NOTE: Returns NULL when none exist,
// pass NULL ended array
optional<const string&> GetFirstExistingFolder(const vector<string> folders);
vector<string> GetFolderFiles(const string folder, const string ext);

};  // namespace basalt