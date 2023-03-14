#include "basalt_extra.h"

const uint WIDTH = 640;
const uint HEIGHT = 480;

static Texture colorSprite;
static Texture foxSprite;
static Texture blockSprite;
static Texture smallBlockSprite;

#include "temple_defs.c"
#include "temple_levels.c"

void InitializeGame() {
    colorSprite = LoadTexture(SPR_RGBA);
    foxSprite = LoadTexture(SPR_PLAYER_FOX);
    blockSprite = LoadTexture(SPR_BLOCK);
    smallBlockSprite = LoadTexture(TILE_BLOCK_SMALL);
}

void DisposeGame() { WARN("TODO: dispose game properly"); }

void UpdateAndRenderGame(Texture canvas, float delta) {

    static const TextureSheet *anims[] = {
        &ANIM_FOX_IDLE, &ANIM_FOX_WALK, &ANIM_FOX_STOP,
        &ANIM_FOX_JUMP, &ANIM_FOX_LAND, NULL,
    };

    RenderWeirdTestGradient(canvas);
    BlitTexture(canvas, colorSprite, 300, 250);

    // draw each fox animation for testing
    static int foxFrame = 0;
    TextureSheet sheet = *anims[foxFrame];
    BlitTextureSheet(canvas, sheet, foxFrame, (Vec2){10, 10});
    foxFrame++;
    if (anims[foxFrame] == NULL) {
        foxFrame = 0;
    }

    BlitTexture(canvas, blockSprite, 10, foxSprite.height + 10);
    BlitTexture(canvas, smallBlockSprite, blockSprite.width + 10,
                foxSprite.height + 10);
}
