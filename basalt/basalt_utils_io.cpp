#include "basalt_utils_io.hpp"

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

void ReplaceAll(std::string& str, const std::string& from, const std::string& to)
{
   if(from.empty())
       return;
   size_t start_pos = 0;
   while((start_pos = str.find(from, start_pos)) != std::string::npos) {
       str.replace(start_pos, from.length(), to);
       start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
   }
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

    // If the file doesn't exist return empty vector.
    if (!fs::exists(folder)) {
        return {};
    }

    // Iterate over the files in the specified folder
    for (const auto& entry : fs::directory_iterator(folder)) {
        // Check if the current entry is a regular file and has the specified extension
        if (entry.is_regular_file() && (!path.empty() && entry.path().extension() == ext)) {
            // Add the file name to the vector
            files.push_back(entry.path().filename().string());
        }
    }

    return files;
}

bool FolderContainsFile(string folderPath) {
    auto files = GetFolderFiles(folderPath);
}