#include "basalt.h"

typedef struct {
    int width;
    int height;
    uint32_t* pixels;
} Texture;

Texture LoadTexture(unsigned char* data);

// core assets
extern unsigned int SPR_RGBA[];
extern unsigned int SPR_PLAYER_FOX[];
extern unsigned int SPR_BLOCK[];
extern unsigned int TILE_BLOCK_SMALL[];
