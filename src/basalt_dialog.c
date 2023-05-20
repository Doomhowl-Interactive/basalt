#include "basalt_extra.h"
#define DIALOG_SKIN

BASALT StringArray ExtractDialogKeywords(const char* line)
{
    StringArray result = InitStringArray();

    usize keywordStart = 0;
    for (usize i = 0; i < strlen(line); i++)
    {
        char token = line[i];
        if (token == '{')
        {
            if (keywordStart > 0)
            {
                ERR("Duplicate start brace for dialog line %s", line);
            }
            keywordStart = i+1;
        }
        else if (token == '}' || token == '|')
        {
            if (keywordStart == 0)
            {
                ERR("No start brace found for dialog line %s", line);
            }

            // copy the keyword over and place it into the stringarray
            usize len = i-keywordStart;
            char* slice = malloc(len+1);
            memcpy(slice, &line[keywordStart], len);
            slice[len] = '\0';
            StoreString(&result, slice);
            free(slice);

            if (token == '}') // reached the end
            {
                break;
            }
            else
            {
                keywordStart = i+1;
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
    char* line = strtok(buffer,SEP);
    while (line != NULL)
    {
        StripText(line);
        StoreString(&result, line);
        line = strtok(NULL, SEP);
    }

    free(buffer);
    return result;
}

DIALOG_SKIN void DrawDefaultDialogBox(const char* dialog, StringArray keywords,
                                      Texture canvas, float timeSince)
{

}

BASALT bool UpdateAndRenderDialogBoxes(Texture cancas, float delta)
{
    // todo
    return false;
}

BASALT void StartDialogSequence(const char* dialog)
{

}

BASALT void RegisterDialogSequence(const char* name, const char* lines)
{

}
