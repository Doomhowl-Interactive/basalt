#include <string>
#include <vector>
#include <unordered_map>

#include "basalt_extra.h"
#include "basalt_extra.hpp"
#include <stdexcept>
#include <optional>

#define DIALOG_SKIN

typedef bool (*DialogBoxDrawerFunc)(const char* dialog, std::vector<std::string> keywords,
                                    Texture canvas, float timeSince);

struct DialogLine {
    std::string text;
    std::vector<std::string> keywords;
};

struct DialogSequence {
    std::string name;
    std::vector<DialogLine> lines;

    std::optional<DialogLine> getDialogLine(size_t index)
    {
        try
        {
            return std::optional(lines.at(index));
        }
        catch (std::out_of_range e)
        {
            return std::optional<DialogLine>();
        }
    }
};

struct GameDialog {
    std::unordered_map<std::string, DialogSequence> sequences;

    std::string curSequence;
    bool isSpeaking;
    double startTime;
    size_t lineIndex;

    std::optional<DialogSequence> getCurrentSequence()
    {
        return getSequence(curSequence);
    }

    std::optional<DialogSequence> getSequence(std::string& name)
    {
        try {
            DialogSequence sequence = sequences.at(name);
            return std::optional(sequence);
        } catch (std::out_of_range e) {
            return std::optional<DialogSequence>();
        }
    }
};

static GameDialog Dialog = {};

// HACK: bridge, Destroy the StringArray and return it as std::vector
std::vector<std::string> StringArrayToVector(StringArray& arr)
{
    std::vector<std::string> lines;
    for (size_t i = 0; i < arr.count; i++)
    {
        std::string line = std::string(arr.strings[i]);
        lines.push_back(line);
    }
    DisposeStringArray(&arr);
    return lines;
}

DIALOG_SKIN bool DrawDefaultDialogBox(const char* dialog, std::vector<std::string> keywords, Texture canvas, float timeSince)
{
    if (IsKeyPressed(SDLK_x) || IsKeyPressed(SDLK_SPACE) || IsKeyPressed(SDLK_RETURN)) {
        return true;
    }

    const int margin = 10;
    const int padding = 10;
    const int height = 80;

    const int topX = margin;
    const int topY = Game.height - height - margin;

    DrawRectangle(canvas, topX, topY, Game.width - 2 * margin, height, BLACK);
    DrawText(canvas, dialog, topX + padding, topY + padding, WHITE);

    DrawText(canvas, "this font looks ass for now also missing characters!", topX + padding + 250, topY + height - padding, YELLOW);

    return false;
}

BASALT void StartDialogSequence(const char* dialog)
{
    if (!Dialog.getSequence(std::string(dialog)))
    {
        SDL_LogWarn(0, "No registered dialog sequences named %s !", dialog);
        return;
    }

    if (!Dialog.curSequence.empty() && TextIsEqualNoCase(Dialog.curSequence.c_str(), dialog))
    {
        SDL_LogWarn(0, "Already speaking, can't start dialog %s", dialog);
        return;
    }

    // start speaking
    Dialog.curSequence = dialog;
    Dialog.isSpeaking = true;
    Dialog.startTime = GetTimeElapsed();
    SDL_Log("> Started dialog sequence %s", dialog);
}

func bool UpdateAndRenderCustomDialogBoxes(Texture canvas, DialogBoxDrawerFunc drawingFunc)
{
    if (!Dialog.isSpeaking)
    {
        return false;
    }

    auto curSequenceOpt = Dialog.getCurrentSequence();
    if (!curSequenceOpt) {
        SDL_LogWarn(0,"Invalid dialog state, reverting...");
        Dialog.isSpeaking = false;
        return false;
    }
    DialogSequence curSequence = curSequenceOpt.value();

    auto lineOpt = curSequence.getDialogLine(Dialog.lineIndex);
    if (!lineOpt)
    {
        SDL_LogWarn(0,"No dialog line with index %ul", Dialog.lineIndex);
        return false;
    }
    DialogLine line = lineOpt.value();

    // advance to next line if confirmed
    float elapsed = (float)(GetTimeElapsed() - Dialog.startTime);
    if ((*drawingFunc)(line.text.c_str(), line.keywords, canvas, elapsed))
    {
        Dialog.lineIndex++;
        if (Dialog.lineIndex >= curSequence.lines.size())
        {
            Dialog.isSpeaking = false;
            SDL_Log("< Ended dialog sequence %s", Dialog.curSequence.c_str());
        }
        else
        {
            SDL_Log(">>> %s", Dialog.curSequence.c_str());
        }
    }
    return true;
}

BASALT bool UpdateAndRenderDialogBoxes(Texture canvas)
{
    return UpdateAndRenderCustomDialogBoxes(canvas, DrawDefaultDialogBox);
}

BASALT void RegisterDialogSequence(const char* name, const char* lines)
{
    DialogSequence seq = DialogSequence();
    seq.name = name;

    // extract lines from compact 'lines' parameter
    StringArray sa = ExtractDialogLines(lines);
    for (size_t i = 0; i < sa.count; i++)
    {
        const char* text = sa.strings[i];
        StringArray keywordsC = ExtractDialogKeywords(text);

        FormatDialogLine(text);

        DialogLine line;
        line.text = text;

        // HACK: bridge
        line.keywords = StringArrayToVector(keywordsC);
        seq.lines.push_back(line);
    }
    DisposeStringArray(&sa);

    Dialog.sequences.insert({ name, seq });

    SDL_LogDebug(0, "Registered dialog sequence %s with %ul lines.", name, seq.lines.size());
}
