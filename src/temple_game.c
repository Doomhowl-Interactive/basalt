#include "basalt.h"
#include "basalt_extra.h"

const uint WIDTH = 640;
const uint HEIGHT = 480;

static Texture colorSprite;
static Texture foxSprite;
static Texture blockSprite;
static Texture smallBlockSprite;

#define SKY_COLOR 0x00C8FF

#include "temple_assets.c"
#include "temple_levels.c"

pubfunc void InitializeGame() {
    foxSprite = LoadTexture(SPR_PLAYER_FOX);
    blockSprite = LoadTexture(SPR_BLOCK);
}

pubfunc void DisposeGame() {
    WARN("TODO: dispose game properly");
}

pubfunc void UpdateAndRenderGame(Texture canvas, float delta) {

    static const TextureSheet *anims[] = {
        &ANIM_FOX_IDLE, &ANIM_FOX_WALK, &ANIM_FOX_STOP,
        &ANIM_FOX_JUMP, &ANIM_FOX_LAND, NULL,
    };

    // ClearTexture(canvas, SKY_COLOR);

    DrawWeirdTestGradient(canvas);

    // draw each fox animation for testing
    static int foxFrame = 0;
    int offsetX = 10;
    for (int i = 0; anims[i] != NULL; i++){
        TextureSheet sheet = *anims[i];
        assert(sheet.texture);
        Vec2 pos = {offsetX, 10};
        DrawTextureSheet(canvas, sheet, foxFrame, pos);
        offsetX += 50;
    }
    foxFrame++;

    // draw other sprites
    DrawTexture(canvas, blockSprite, 150, 50);
}
