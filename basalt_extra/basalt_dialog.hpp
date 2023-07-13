#pragma once

// turn into library basalt_extra.lib

#include <vector>
#include <string>

#include "basalt_graphics.hpp"

namespace basalt {
using namespace std;
class DialogLine {
   public:
    string text;
    vector<string> keywords;

    DialogLine(string text);

   private:
    // Removes all markup and markers from a dialog string
    string& formatLine(string& line);

    vector<string>& extractKeywords(string& line);
    static vector<DialogLine>& extract(string& lines);
};

struct DialogSequence {
    vector<DialogLine> lines;

    // Creates a dialog sequence from markupped lines (end each line with a backslash)
    DialogSequence(vector<string>& lines);
    DialogSequence(string lines);
};

class Dialog {
   public:
    // Draws dialog boxes (default style), returns true while in a dialog sequence
    bool UpdateAndRender(Texture canvas);

    // Draws dialog boxes (custom style), returns true while in a dialog sequence
    // bool UpdateAndRenderCustomDialogBoxes(Texture canvas, DialogBoxDrawerFunc drawingFunc);

    void StartSequence(DialogSequence dialog);

    bool IsSpeaking();
    const DialogSequence& GetCurrentSequence() const;

   private:
    DialogSequence curSequence;
    bool isSpeaking;
    double startTime;
    size_t lineIndex;
};

}  // namespace basalt