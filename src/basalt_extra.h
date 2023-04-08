#ifndef BASALT_EXTRA_H
#define BASALT_EXTRA_H

#include "basalt.h"

#define STRING(name) #name

enumdef(PlayerPose) {
    POSE_IDLE,
    POSE_WALK,
    POSE_SLIDE,
    POSE_JUMP,
    POSE_FALL,
    POSE_DUCK,
};

#define PLAYER_POSE_LIST                                                       \
    {                                                                          \
        STRING(POSE_IDLE), STRING(POSE_WALK), STRING(POSE_SLIDE),              \
            STRING(POSE_JUMP), STRING(POSE_FALL), STRING(POSE_DUCK),           \
    }

enumdef(PlaybackMode) {
    PLAY_LOOP,
    PLAY_PING_PONG,
    PLAY_ONCE,
};

class(TextureSheet) {
    const char *name; // description
    Texture* texture;  // attached texture
    Vec2 origin;      // top left of sector
    Vec2 cellSize;    // size of each cell
    uint count;       // number of cells in anim
    PlaybackMode mode;
    float fps;
};

pubfunc Texture GenerateNoiseTexture(int width, int height, Color bg, Color fg);
pubfunc Texture GenerateNoiseTextureEx(int width, int height, Color bg, Color fg, double scale, int seed);
pubfunc void DrawNoiseRectangle(Texture canvas, Rect region, Vec2 offset, Color bg, Color fg); // WARN: Only noise context at a time for now.

pubfunc void DrawTextureSheet(Texture canvas, TextureSheet sheet, int frame, Vec2 pos);

#endif
