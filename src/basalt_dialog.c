#include "basalt_extra.h"
#define DIALOG_SKIN

BASALT StringArray ExtractDialogKeywords(const char* line)
{

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
