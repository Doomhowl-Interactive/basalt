#pragma once

#include "basalt.h"

#define STRING(name) #name

typedef enum PlayerPose {
    POSE_IDLE,
    POSE_WALK,
    POSE_SLIDE,
    POSE_JUMP,
    POSE_FALL,
    POSE_DUCK,
} PlayerPose;

#define PLAYER_POSE_LIST \
    { \
        STRING(POSE_IDLE), STRING(POSE_WALK), STRING(POSE_SLIDE), STRING(POSE_JUMP), STRING(POSE_FALL), STRING(POSE_DUCK), \
    }

typedef enum PlaybackMode {
    PLAY_LOOP,
    PLAY_PING_PONG,
    PLAY_ONCE,
} PlaybackMode;

typedef struct TextureSheet {
    const char* name;  // description
    Texture* texture;  // attached texture
    Vec2 origin;       // top left of sector
    Vec2 cellSize;     // size of each cell
    uint count;        // number of cells in anim
    PlaybackMode mode;
    float fps;
} TextureSheet;

BASALT Texture GenerateNoiseTexture(int width, int height, Color bg, Color fg);
BASALT Texture GenerateNoiseTextureEx(int width, int height, Color bg, Color fg, double scale, int seed);

BASALT void DrawTextureSheet(Texture canvas, TextureSheet sheet, int frame, Vec2 pos, Color tint);

// basalt_dialog.c
typedef struct DialogLine {
    const char* text;
    const StringArray keywords;
} DialogLine;

#define MAX_LINES_PER_SEQUENCE 128
typedef struct DialogSequence {
    const char* name;
    DialogLine lines[MAX_LINES_PER_SEQUENCE];
    usize lineCount;
} DialogSequence;

// Removes all markup and markers from a dialog string IN PLACE! 
BASALT void FormatDialogLine(const char* line);

BASALT StringArray ExtractDialogKeywords(const char* line);
BASALT StringArray ExtractDialogLines(const char* lines);

typedef bool (*DialogBoxDrawerFunc)(const char* dialog, StringArray keywords, Texture canvas, float timeSince);

// Draws dialog boxes (default style), returns true while in a dialog sequence
BASALT extern bool UpdateAndRenderDialogBoxes(Texture canvas);

// Draws dialog boxes (custom style), returns true while in a dialog sequence
BASALT bool UpdateAndRenderCustomDialogBoxes(Texture canvas, DialogBoxDrawerFunc drawingFunc);

// Creates a dialog sequence from markupped lines (end each line with a backslash)
BASALT void RegisterDialogSequence(const char* name, const char* lines);
BASALT void StartDialogSequence(const char* dialog);
BASALT void ClearDialogSequences();
