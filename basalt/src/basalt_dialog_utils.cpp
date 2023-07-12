#include <string>
#include <malloc.h>

#include "basalt_dialog.hpp"
#include "basalt_utils.hpp"
#include "basalt_console.hpp"

using namespace std;

string& DialogLine::formatLine(string& line)
{
    // remove everything between braces including the braces
    size_t start = line.find('{');
    size_t end = line.find('}');
    while (start != string::npos && end != string::npos) {
        line.erase(start, end - start + 1);
        start = line.find('{');
        end = line.find('}');
    }
    return line;
}

std::vector<std::string>& DialogLine::extractKeywords(string& line)
{
    std::vector<std::string> keywords;
    size_t keywordStart = 0;
    for (size_t i = 0; i < line.size(); i++) {
        char token = line[i];
        if (token == '{') {
            if (keywordStart > 0) {
                ERR("Duplicate start brace for dialog line %s", line);
            }
            keywordStart = i + 1;
        } else if (token == '}' || token == '|') {
            if (keywordStart == 0) {
                ERR("No start brace found for dialog line %s", line);
            }

            // TODO: rewrite
            // copy the keyword over and place it into the stringarray
            size_t len = i - keywordStart;
            char* slice = new char[len + 1];
            memcpy(slice, &line[keywordStart], len);
            slice[len] = '\0';
            keywords.push_back(slice);
            free(slice);

            if (token == '}')  // reached the end
            {
                break;
            } else {
                keywordStart = i + 1;
            }

            delete[] slice;
        }
    }
    return keywords;
}

StringArray ExtractDialogLines(const char* lines)
{
    char* buffer = CloneText(lines);

    StringArray result = InitStringArray();
    const char* SEP = "\\";
    char* line = strtok(buffer, SEP);
    while (line != NULL) {
        StripText(line);
        StoreString(&result, line);
        line = strtok(NULL, SEP);
    }

    free(buffer);
    return result;
}