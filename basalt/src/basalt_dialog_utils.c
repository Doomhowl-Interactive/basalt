#include "basalt_extra.h"

BASALT void FormatDialogLine(const char* line)
{
    char* clone = (char*)CloneText(line);  // TODO: lazy

    usize j = 0;
    bool penDown = true;
    for (usize i = 0; i < strlen(clone); i++) {
        char token = clone[i];
        if (token == '{') {
            penDown = false;
        } else if (token == '}') {
            penDown = true;
            continue;
        }
        if (penDown) {
            ((char*)line)[j++] = token;
        }
    }

    // null terminate so we don't have any garbage
    ((char*)line)[j] = '\0';

    free(clone);
}

BASALT StringArray ExtractDialogKeywords(const char* line)
{
    StringArray result = InitStringArray();

    usize keywordStart = 0;
    for (usize i = 0; i < strlen(line); i++) {
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

            // copy the keyword over and place it into the stringarray
            usize len = i - keywordStart;
            char* slice = malloc(len + 1);
            memcpy(slice, &line[keywordStart], len);
            slice[len] = '\0';
            StoreString(&result, slice);
            free(slice);

            if (token == '}')  // reached the end
            {
                break;
            } else {
                keywordStart = i + 1;
            }
        }
    }
    return result;
}

BASALT StringArray ExtractDialogLines(const char* lines)
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