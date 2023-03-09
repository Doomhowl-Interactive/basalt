#include "basalt.h"

typedef struct {
    int width;
    int height;
    color* pixels;
} Texture;

Texture LoadTexture(unsigned char* data);

// core assets
extern uchar SPR_RGBA[];
extern uchar SPR_PLAYER_FOX[];
extern uchar SPR_BLOCK[];
extern uchar TILE_BLOCK_SMALL[];
