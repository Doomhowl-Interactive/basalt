#pragma once

#include "basalt.h"

#define STRING(name) #name

typedef enum PlayerPose {
    POSE_IDLE, POSE_WALK, POSE_SLIDE, POSE_JUMP, POSE_FALL, POSE_DUCK,
} PlayerPose;

#define PLAYER_POSE_LIST \
 { \
  STRING(POSE_IDLE), STRING(POSE_WALK), STRING(POSE_SLIDE), STRING(POSE_JUMP), STRING(POSE_FALL), STRING(POSE_DUCK), \
 }

typedef enum PlaybackMode{
    PLAY_LOOP,
    PLAY_PING_PONG,
    PLAY_ONCE,
} PlaybackMode;

typedef struct TextureSheet
{
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
    const char* text;  // split multiple lines with backslashes!
    uint speaker;
    uint expression;   // default neutral 0
    float customSpeed; // default speed 1.0
} DialogLine;

typedef struct DialogSequence {
    const char* name;
    DialogLine lines[];
} DialogSequence;

typedef bool (*DialogBoxDrawerFunc)(const char* dialog, uint speaker, float timeSince);
BASALT bool UpdateAndRenderDialogBoxes(Texture canvas, float delta); // returns true while speaking
BASALT void RegisterDialogSequence(const char* name, const char* lines);
BASALT void StartDialogSequence(const char* dialog);
