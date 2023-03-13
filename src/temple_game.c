#include "basalt.h"

const uint WIDTH = 640;
const uint HEIGHT = 480;

static Texture colorSprite;
static Texture foxSprite;
static Texture blockSprite;
static Texture smallBlockSprite;

void InitializeGame() {
    colorSprite = LoadTexture(SPR_RGBA);
    foxSprite = LoadTexture(SPR_PLAYER_FOX);
    blockSprite = LoadTexture(SPR_BLOCK);
    smallBlockSprite = LoadTexture(TILE_BLOCK_SMALL);
}

void DisposeGame() {
    WARN("TODO: dispose game properly");
}

void UpdateAndRenderGame(Texture canvas, float delta) {

    RenderWeirdTestGradient(canvas);
    RenderSprite(canvas, colorSprite, 300, 250);
    RenderSprite(canvas, foxSprite, 10, 10);
    RenderSprite(canvas, blockSprite, 10, foxSprite.height + 10);
    RenderSprite(canvas, smallBlockSprite, blockSprite.width + 10,
                 foxSprite.height + 10);
}
