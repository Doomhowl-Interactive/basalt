#include "basalt.h"
#include "basalt_extra.h"
#include "temple_assets.h"

const uint WIDTH = 640;
const uint HEIGHT = 480;
const char* GAME_TITLE = "Basalt Engine";

const uint TPS = 25;
const uint MAX_ENTITIES = 1024;

static Texture colorSprite;
static Texture foxSprite;
static Texture blockSprite;
static Texture smallBlockSprite;

#define SKY_COLOR 0x00C8FF

#include "temple_assets.c"
#include "temple_levels.c"

BASALT void InitializeGame()
{
    foxSprite = RequestTexture(SPR_PLAYER_FOX);
    blockSprite = RequestTexture(SPR_BLOCK);
    smallBlockSprite = RequestTexture(TILE_BLOCK_SMALL);
    colorSprite = RequestTexture(SPR_RGBA);
}

BASALT void DisposeGame()
{
    WARN("TODO: dispose game properly");
}

BASALT void UpdateAndRenderGame(Texture canvas, float delta)
{
    static const TextureSheet* anims[] = { &ANIM_FOX_IDLE, &ANIM_FOX_WALK, &ANIM_FOX_STOP, &ANIM_FOX_JUMP, &ANIM_FOX_LAND };
    static float timers[5];
    static int indices[5];

    // ClearTexture(canvas, SKY_COLOR);
    DrawWeirdTestGradient(canvas);

    // draw each fox animation for testing
    int offsetX = 10;
    for (int i = 0; i < 5; i++) {
        const TextureSheet sheet = *anims[i];
        assert(sheet.texture);

        Vec2 pos = { offsetX, 10 };
        DrawTextureSheet(canvas, sheet, indices[i], pos, WHITE);
        offsetX += 50;

        timers[i] += delta;
        if (timers[i] > 1.f / sheet.fps) {
            timers[i] = 0.f;
            indices[i]++;
        }
    }

    // TODO: use raylib's color constants
    DrawRectangle(canvas, 50 + 00, 250 + 00, 150, 150, RGB(255, 0, 0));
    DrawRectangle(canvas, 50 + 20, 250 + 20, 150, 150, RGB(0, 255, 0));
    DrawRectangle(canvas, 50 + 40, 250 + 40, 150, 150, RGB(0, 0, 255));
    DrawRectangle(canvas, 50 + 60, 250 + 60, 150, 150, RGBA(0, 0, 0, 255));

    DrawRectangle(canvas, 300 + 00, 250 + 00, 150, 150, 0xFF0000FF);
    DrawRectangle(canvas, 300 + 20, 250 + 20, 150, 150, 0x00FF00FF);
    DrawRectangle(canvas, 300 + 40, 250 + 40, 150, 150, 0x0000FFFF);
    DrawRectangle(canvas, 300 + 60, 250 + 60, 150, 150, 0x000000FF);

    // draw other sprites
    DrawTexture(canvas, blockSprite, 150, 50, WHITE);
    DrawTexture(canvas, smallBlockSprite, 350, 50, WHITE);
    DrawTexture(canvas, colorSprite, 450, 50, WHITE);

    // draw rectangle at cursor for testing
    Point mouse = GetMousePosition();
    DrawRectangle(canvas, mouse.x, mouse.y, 32, 32, 0xFFAAAAFF);
}
