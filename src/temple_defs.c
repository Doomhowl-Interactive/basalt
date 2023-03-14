#include "basalt_extra.h"

#define FOX_CELL_SIZE {17,32}
#define FOX_ANIM_FPS 15.f

extern Texture foxSprite;

const TextureSheet ANIM_FOX_IDLE = {
    STRING(ANIM_FOX_IDLE),
    &foxSprite,
    {68,64},
    FOX_CELL_SIZE,
    1,
    PLAY_LOOP,
    FOX_ANIM_FPS,
};

const TextureSheet ANIM_FOX_WALK = {
    STRING(ANIM_FOX_WALK),
    &foxSprite,
    {0,0},
    FOX_CELL_SIZE,
    3,
    PLAY_PING_PONG,
    FOX_ANIM_FPS,
};

const TextureSheet ANIM_FOX_STOP = {
    STRING(ANIM_FOX_STOP),
    &foxSprite,
    {68, 0},
    FOX_CELL_SIZE,
    2,
    PLAY_ONCE,
    FOX_ANIM_FPS * 0.5,
};

const TextureSheet ANIM_FOX_JUMP = {
    STRING(ANIM_FOX_JUMP),
    &foxSprite,
    {0, 32},
    FOX_CELL_SIZE,
    1,
    PLAY_LOOP,
    0.f,
};

const TextureSheet ANIM_FOX_LAND = {
    STRING(ANIM_FOX_LAND),
    &foxSprite,
    {17, 32},
    FOX_CELL_SIZE,
    1,
    PLAY_LOOP,
    0.f,
};
