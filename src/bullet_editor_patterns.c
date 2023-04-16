#include <stdio.h>
#include "basalt.h"
#include "basalt_extra.h"
#include "bullet_common.h"

class(PatternEditor)
{
    BulletSpawner* spawner;
    Texture buffer;
    Scene scene;
};
static PatternEditor Context = { 0 };

void InitPatternEditor()
{
    assert(GetBulletPatternCount() > 0);

    // Allocate new canvas
    Context.buffer = InitTexture(WIDTH, HEIGHT);

    // Spawn bullet spawner in the center of the scene
    Entity* spawner = CreateEntity(&Context.scene);
    SetEntityCenter(spawner, WIDTH * 0.5f, HEIGHT * 0.5f);
    Context.spawner = &spawner->bulletSpawners[0];
    Context.spawner->patternToSpawn = GetBulletPattern(0);
    Context.spawner->interval = 0.2f;
    Context.spawner->normal = (Vec2){ 0, 0.5f };
}

func void DrawScreenGrid(Texture canvas, uint cellWidth, uint cellHeight, Color color)
{
    for (uint y = 0; y < HEIGHT; y += cellHeight) {
        DrawLine(canvas, 0, y, WIDTH, y, color);
    }

    for (uint x = 0; x < WIDTH; x += cellWidth) {
        DrawLine(canvas, x, 0, x, HEIGHT, color);
    }
}

BULLET void UpdateAndRenderPatternEditor(Texture canvas, float delta)
{
    if (Context.spawner == NULL) {
        InitPatternEditor();
    }

    ClearTexture(Context.buffer, 0x181818FF);

    // Draw grid
    const uint gridSize = 32;
    DrawScreenGrid(Context.buffer, gridSize, gridSize, 0x999999FF);

    UpdateAndRenderScene(&Context.scene, Context.buffer, delta);

    // Draw info
    Rect contentRegion = GetEditorTabContentRegion();

    char gridSizeText[16];
    sprintf(gridSizeText, "%ux%u", gridSize, gridSize);
    DrawText(Context.buffer, gridSizeText, 10, contentRegion.y + 10, PURPLE);

    // Draw result
    DrawTextureEx(canvas, Context.buffer, contentRegion.x, contentRegion.y, R2(contentRegion), WHITE);
}
