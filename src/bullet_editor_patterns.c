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
static PatternEditor Context = { 0 };

void InitPatternEditor()
{
    assert(GetBulletPatternCount() > 0);

    // Allocate new canvas
    Context.buffer = InitTexture(WIDTH, HEIGHT);

    // Spawn bullet spawner in the center of the scene
    Context.spawnerEntity = CreateEntity(&Context.scene);
    SetEntityCenter(Context.spawnerEntity, WIDTH * 0.5f, HEIGHT * 0.5f);
    Context.spawner = &Context.spawnerEntity->bulletSpawners[0];
    Context.spawner->patternToSpawn = GetBulletPattern(0);
    Context.spawner->interval = 2.f;
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
    static usize PatternCount = 0;
    if (PatternCount == 0) {
        PatternCount = GetBulletPatternCount();
    }

    if (Context.spawner == NULL) {
        InitPatternEditor();
    }

    ClearTexture(Context.buffer, 0x181818FF);

    // Aim bulletspawner at the cursor
    Point mouse = GetMousePosition();
    Vec2 center = GetEntityCenter(Context.spawnerEntity);
    Vec2 direction = { mouse.x - center.x, mouse.y - center.y };
    Vec2 norm = Vec2Normalize(direction);
    Context.spawner->normal = norm;

    // Draw grid
    const uint gridSize = 32;
    DrawScreenGrid(Context.buffer, gridSize, gridSize, 0x999999FF);

    UpdateAndRenderScene(&Context.scene, Context.buffer, delta);

    // Draw info
    Rect contentRegion = GetEditorTabContentRegion();

    const BulletPattern* curPattern = GetBulletPattern(Context.patternIndex);

    char infoText[512];
    sprintf(infoText, "%ux%u\n%s\n\n", gridSize, gridSize, curPattern->name);

    // Draw list of bullet patterns
    for (usize i = 0; i < PatternCount; i++) {
        const BulletPattern* pattern = GetBulletPattern(i);
        // FIXME: SLOW
        if (i == Context.patternIndex) {
            strcat(infoText, "SELECTED ");
        }
        strcat(infoText, pattern->name);
        strcat(infoText, "\n");
    }

    if (IsKeyPressed(KEY_J)) {
        Context.patternIndex++;
    }
    if (IsKeyPressed(KEY_K)) {
        Context.patternIndex--;
    }
    Context.patternIndex %= PatternCount;
    Context.spawner->patternToSpawn = curPattern;

    DrawText(Context.buffer, infoText, 10, contentRegion.y + 10, PURPLE);

    // Draw result
    DrawTextureEx(canvas, Context.buffer, contentRegion.x, contentRegion.y, R2(contentRegion), WHITE);
}
