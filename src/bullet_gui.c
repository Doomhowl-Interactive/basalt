#include "basalt.h"
#include "bullet_common.h"

typedef struct GUIContext {
    const LevelInfo* info;
    float timePassed;
} GUIContext;
static GUIContext Context = { 0 };

func void UpdateAndRenderLives(Texture canvas, Entity* player, float delta)
{
    const int size = 64;
    const int border = 10;
    const int spacing = 10;

    Vec2 pos = {
        border,
        HEIGHT - size - border,
    };

    if (player == NULL || !player->isActive) {
        return;
    }

    Texture heartTexture = RequestTexture("spr_gui_heart");
    for (uint i = 0; i < player->maxHealth; i++) {
        DrawTexture(canvas, heartTexture, V2(pos), i < player->health ? WHITE : BLACK);
        pos.x += heartTexture.width + spacing;
    }
}

func void OnLevelEntered(const LevelInfo* info)
{
    INFO("Entered level %s (hooked function)", info->name);
    Context.info = info;
    Context.timePassed = 0.0f;
}

func void DrawLevelTitle(Texture canvas, float delta)
{
    const LevelInfo* level = Context.info;
    if (!level) {
        return;
    }

    if (Context.timePassed < 2.0f) {
        DrawText(canvas, level->name, 150, 150 + Context.timePassed * 10.f, 0xFFFF00FF);
    }
}

BULLET void UpdateAndRenderGUI(Texture canvas, Entity* player, float delta)
{
    DrawLevelTitle(canvas, delta);
    UpdateAndRenderLives(canvas, player, delta);

    // hooks
    RunLevelEnterHook(OnLevelEntered);

    Context.timePassed += delta;
}
