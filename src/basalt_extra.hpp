#pragma once

#include "basalt.h"

// basalt_dialog.cpp

#ifdef __cplusplus
extern "C" {
#endif

// Removes all markup and markers from a dialog string IN PLACE!
BASALT void FormatDialogLine(const char* line);

BASALT StringArray ExtractDialogKeywords(const char* line);
BASALT StringArray ExtractDialogLines(const char* lines);

// Draws dialog boxes (default style), returns true while in a dialog sequence
BASALT bool UpdateAndRenderDialogBoxes(Texture canvas);

// Draws dialog boxes (custom style), returns true while in a dialog sequence
// BASALT bool UpdateAndRenderCustomDialogBoxes(Texture canvas, DialogBoxDrawerFunc drawingFunc);

// Creates a dialog sequence from markupped lines (end each line with a backslash)
BASALT void RegisterDialogSequence(const char* name, const char* lines);
BASALT void StartDialogSequence(const char* dialog);

#ifdef __cplusplus
}
#endif
