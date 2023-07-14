#include <string>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <functional>

#include "basalt.h"
#include "basalt_dialog.hpp"

using namespace std;

typedef function<bool(Texture, DialogSequence, vector<string>, float)> DialogBoxDrawerFunc;

DialogLine::DialogLine(string text)
{
    this->text = text;
    this->keywords = extractKeywords(text);
}

bool Dialog::UpdateAndRender(Texture canvas)
{
    return false;
}

void Dialog::StartSequence(DialogSequence dialog)
{
}

bool Dialog::IsSpeaking()
{
    return false;
}

const DialogSequence& Dialog::GetCurrentSequence() const
{
    return curSequence;
}

bool DrawDefaultDialogBox(Texture canvas,
                          const char* dialog,
                          vector<string> keywords,
                          float timeSince)
{
    if (IsKeyPressed(KEY_x) || IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_RETURN)) {
        return true;
    }

    const int margin = 10;
    const int padding = 10;
    const int height = 80;

    const int topY = Game.height - height - margin;

    // first keyword is the avatar collection name
    // second keyword is the emotion used
    // render avatar texture
    if (keywords.size() > 0) {
        string avatar = keywords[0];
        string emotion = keywords.size() > 1 ? keywords[1] : "neutral";

        const int size = height;
        const char* avatarPath = FormatText("spr_dialog_%s_%s", avatar.c_str(), emotion.c_str());

        Texture avatarTexture = RequestTexture(avatarPath);  // TODO: Add placeholder texture
        canvas.BlitScaled(avatarTexture, margin, topY, size, size, WHITE);

        canvas.DrawRectangle(
            margin + height + padding, topY, Game.width - 2 * margin * 2 - height, height, BLACK);
        canvas.DrawText(dialog, margin + height + padding * 2, topY + padding, WHITE);

        string info = emotion + "\n" + avatarPath;
        canvas.DrawText(info, 150, 300, GREEN);
    } else {
        canvas.DrawRectangle(margin, topY, Game.width - 2 * margin, height, BLACK);
        canvas.DrawText(dialog, margin + padding, topY + padding, WHITE);
    }
    return false;
}

void Dialog::StartSequence(DialogSequence dialog)
{
    DialogSequence sequence;
    try {
        sequence = Dialog.getSequence(string(dialog));
    } catch (out_of_range e) {
        WARN("No registered dialog sequences named %s !", dialog);
        return;
    }

    if (!Dialog.curSequence.empty() && TextIsEqualNoCase(Dialog.curSequence.c_str(), dialog)) {
        WARN("Already speaking, can't start dialog %s", dialog);
        return;
    }

    // start speaking
    curSequence = dialog;
    isSpeaking = true;
    startTime = GetTimeElapsed();
    INFO("> Started dialog sequence %s", dialog);
}

static bool UpdateAndRenderCustomDialogBoxes(Texture canvas, DialogBoxDrawerFunc drawingFunc)
{
    if (!Dialog.isSpeaking) {
        return false;
    }

    DialogSequence curSequence;
    try {
        curSequence = Dialog.getCurrentSequence();
    } catch (out_of_range e) {
        WARN("Invalid dialog state, reverting...");
        Dialog.isSpeaking = false;
        return false;
    }

    DialogLine line;
    try {
        line = curSequence.getDialogLine(Dialog.lineIndex);
    } catch (out_of_range e) {
        WARN("No dialog line with index %zu", Dialog.lineIndex);
        return false;
    }

    // advance to next line if confirmed
    float elapsed = (float)(GetTimeElapsed() - Dialog.startTime);
    if ((*drawingFunc)(line.text.c_str(), line.keywords, canvas, elapsed)) {
        Dialog.lineIndex++;
        if (Dialog.lineIndex >= curSequence.lines.size()) {
            Dialog.isSpeaking = false;
            DEBUG("< Ended dialog sequence %s", Dialog.curSequence.c_str());
        } else {
            DEBUG(">>> %s", Dialog.curSequence.c_str());
        }
    }
    return true;
}

bool UpdateAndRenderDialogBoxes(Texture canvas)
{
    return UpdateAndRenderCustomDialogBoxes(canvas, DrawDefaultDialogBox);
}

void RegisterDialogSequence(const char* name, const char* lines)
{
}

bool DialogIsSpeaking()
{
    return Dialog.isSpeaking;
}

}  // namespace basalt