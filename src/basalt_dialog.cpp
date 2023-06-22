#include <string>
#include <vector>
#include <unordered_map>

#include "basalt_extra.h"
#include "basalt_extra.hpp"
#include <stdexcept>
#include <optional>

#define DIALOG_SKIN

typedef bool (*DialogBoxDrawerFunc)(const char* dialog, std::vector<std::string> keywords, Texture canvas, float timeSince);

struct DialogLine {
    std::string text;
    std::vector<std::string> keywords;
};

struct DialogSequence {
    std::string name;
    std::vector<DialogLine> lines;

    DialogLine getDialogLine(size_t index) const
    {
        return lines.at(index);
    }
};

struct Dialog {
    std::unordered_map<std::string, DialogSequence> sequences;

    std::string curSequence;
    bool isSpeaking;
    double startTime;
    size_t lineIndex;

    const DialogSequence& getCurrentSequence() const
    {
        return getSequence(curSequence);
    }

    const DialogSequence& getSequence(const std::string& name) const
    {
        return sequences.at(name);
    }
};

static Dialog Dialog = {};

// HACK: bridge, Destroy the StringArray and return it as std::vector
std::vector<std::string> StringArrayToVector(StringArray& arr)
{
    std::vector<std::string> lines;
    for (size_t i = 0; i < arr.count; i++) {
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

    const int topY = Game.height - height - margin;

    // first keyword is the avatar collection name
    // second keyword is the emotion used
    // render avatar texture
    if (keywords.size() > 0)
    {
        std::string avatar = keywords[0];
        std::string emotion = keywords.size() > 1 ? keywords[1]:"neutral";

        const int size = height;
        const char* avatarPath = FormatText("spr_dialog_%s_%s", avatar.c_str(), emotion.c_str());

        Texture avatarTexture = RequestTexture(avatarPath); // TODO: Add placeholder texture
        DrawTextureScaled(canvas, avatarTexture, margin, topY, size, size, WHITE);

        DrawRectangle(canvas, margin + height + padding, topY, Game.width - 2 * margin * 2 - height, height, BLACK);
        DrawText(canvas, dialog, margin + height + padding * 2, topY + padding, WHITE);

        std::string info = emotion + "\n" + avatarPath;
        DrawText(canvas, info.c_str(), 150, 300, GREEN);
    }
    else
    {
        DrawRectangle(canvas, margin, topY, Game.width - 2 * margin, height, BLACK);
        DrawText(canvas, dialog, margin + padding, topY + padding, WHITE);
    }
    return false;
}

BASALT void StartDialogSequence(const char* dialog)
{
    DialogSequence sequence;
    try {
        sequence = Dialog.getSequence(std::string(dialog));
    } catch (std::out_of_range e) {
        SDL_LogWarn(0, "No registered dialog sequences named %s !", dialog);
        return;
    }

    if (!Dialog.curSequence.empty() && TextIsEqualNoCase(Dialog.curSequence.c_str(), dialog)) {
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
    if (!Dialog.isSpeaking) {
        return false;
    }

    DialogSequence curSequence;
    try {
        curSequence = Dialog.getCurrentSequence();
    } catch (std::out_of_range e) {
        SDL_LogWarn(0, "Invalid dialog state, reverting...");
        Dialog.isSpeaking = false;
        return false;
    }

    DialogLine line;
    try {
        line = curSequence.getDialogLine(Dialog.lineIndex);
    } catch (std::out_of_range e) {
        SDL_LogWarn(0, "No dialog line with index %zu", Dialog.lineIndex);
        return false;
    }

    // advance to next line if confirmed
    float elapsed = (float)(GetTimeElapsed() - Dialog.startTime);
    if ((*drawingFunc)(line.text.c_str(), line.keywords, canvas, elapsed)) {
        Dialog.lineIndex++;
        if (Dialog.lineIndex >= curSequence.lines.size()) {
            Dialog.isSpeaking = false;
            SDL_Log("< Ended dialog sequence %s", Dialog.curSequence.c_str());
        } else {
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
    for (size_t i = 0; i < sa.count; i++) {
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

    SDL_LogDebug(0, "Registered dialog sequence %s with %zu lines.", name, seq.lines.size());
}
