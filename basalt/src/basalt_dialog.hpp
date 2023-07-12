#pragma once

#include <vector>
#include <string>

struct DialogLine {
   public:
    std::string text;
    std::vector<std::string> keywords;

    DialogLine(std::string text);

   private:
    // Removes all markup and markers from a dialog string
    std::string& formatLine(std::string& line);

    std::vector<std::string>& extractKeywords(std::string& line);
    std::vector<std::string>& extractLines(std::string& lines);
};

struct DialogSequence {
    std::vector<DialogLine> lines;
};

class Dialog {
   public:
    // Draws dialog boxes (default style), returns true while in a dialog sequence
    bool UpdateAndRender(Texture canvas);

    // Draws dialog boxes (custom style), returns true while in a dialog sequence
    // bool UpdateAndRenderCustomDialogBoxes(Texture canvas, DialogBoxDrawerFunc drawingFunc);

    // Creates a dialog sequence from markupped lines (end each line with a backslash)
    DialogSequence RegisterSequence(std::vector<std::string> lines);
    void StartSequence(DialogSequence dialog);

    bool IsSpeaking();
    const DialogSequence& GetCurrentSequence() const;

   private:
    DialogSequence curSequence;
    bool isSpeaking;
    double startTime;
    size_t lineIndex;
};