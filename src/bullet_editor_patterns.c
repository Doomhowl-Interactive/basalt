#include <stdio.h>
#include "basalt.h"
#include "basalt_extra.h"
#include "bullet_common.h"

typedef struct PatternEditor {
    Entity* spawnerEntity;
    BulletSpawner* spawner;
    Texture buffer;
    Scene scene;
    usize patternIndex;
} PatternEditor;
static PatternEditor PATED = { 0 };

void InitPatternEditor()
{
    assert(GetBulletPatternCount() > 0);

    // Allocate new canvas
    PATED.buffer = InitTexture(WIDTH, HEIGHT);

    // Spawn bullet spawner in the center of the scene
    PATED.spawnerEntity = CreateEntity(&PATED.scene);
    SetEntityCenter(PATED.spawnerEntity, WIDTH * 0.5f, HEIGHT * 0.5f);
    PATED.spawner = &PATED.spawnerEntity->bulletSpawners[0];
    PATED.spawner->patternToSpawn = GetBulletPattern(0);
    PATED.spawner->interval = 2.f;
    PATED.spawner->normal = (Vec2){ 0, 0.5f };
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
    static usize PatternCount = 0;
    if (PatternCount == 0) {
        PatternCount = GetBulletPatternCount();
    }

    if (PATED.spawner == NULL) {
        InitPatternEditor();
    }

    ClearTexture(PATED.buffer, 0x181818FF);

    // Aim bulletspawner at the cursor
    Point mouse = GetMousePosition();
    Vec2 center = GetEntityCenter(PATED.spawnerEntity);
    Vec2 direction = { mouse.x - center.x, mouse.y - center.y };
    Vec2 norm = Vec2Normalize(direction);
    PATED.spawner->normal = norm;

    // Draw grid
    const uint gridSize = 32;
    DrawScreenGrid(PATED.buffer, gridSize, gridSize, 0x999999FF);

    UpdateAndRenderScene(&PATED.scene, PATED.buffer, delta);

    // Draw info
    Rect contentRegion = GetEditorTabContentRegion();

    const BulletPattern* curPattern = GetBulletPattern(PATED.patternIndex);

    char* infoText = (char*)FormatText("%ux%u\n%s\n\n", gridSize, gridSize, curPattern->name);

    // Draw list of bullet patterns
    for (usize i = 0; i < PatternCount; i++) {
        const BulletPattern* pattern = GetBulletPattern(i);
        const char* add = FormatText("%s%s\n", (i == PATED.patternIndex) ? "SELECTED " : "", pattern->name);
        infoText = (char*)AppendText(infoText, add);
    }

    if (IsKeyPressed(SDLK_j)) {
        PATED.patternIndex++;
    }
    if (IsKeyPressed(SDLK_k)) {
        PATED.patternIndex--;
    }
    PATED.patternIndex %= PatternCount;
    PATED.spawner->patternToSpawn = curPattern;

    DrawText(PATED.buffer, infoText, 10, contentRegion.y + 10, PURPLE);

    // Draw result
    DrawTextureEx(canvas, PATED.buffer, contentRegion.x, contentRegion.y, R2(contentRegion), WHITE);
}
