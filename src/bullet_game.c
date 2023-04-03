#include "basalt.h"
#include "basalt_extra.h"

const uint WIDTH = 800;
const uint HEIGHT = 600;
const char* GAME_TITLE = "Bullethell game";

const uint MAX_FPS = 900;
const uint TPS = 25;

#define SKY_COLOR 0x000000FF

#include "bullet_entities.c"
#include "bullet_patterns.c"

#define SCENE_GAME              0
#define SCENE_BULLET_PREVIEW    1
#define SCENE_COUNT             2

static uint ActiveSceneID     = 0;
static Scene Scenes[SCENE_COUNT] = { 0 };

pubfunc void InitializeGame()
{
    Entity* player = CreateEntity(&Scenes[SCENE_GAME]);
    Vec2 spawnPos = {WIDTH / 2, HEIGHT / 1.2f};
    InitPlayer(player, spawnPos);
}

pubfunc void DisposeGame()
{
    WARN("TODO: dispose game properly");
}

pubfunc void UpdateAndRenderGame(Texture canvas, float delta)
{
    if (IsKeyPressed(KEY_I))
    {
        ActiveSceneID++;
        if (ActiveSceneID >= SCENE_COUNT)
            ActiveSceneID = 0;
    }
    if (IsKeyPressed(KEY_U))
    {
        ActiveSceneID--;
        if (ActiveSceneID < 0)
            ActiveSceneID = SCENE_COUNT-1;
    }

    Scene* activeScene = &Scenes[ActiveSceneID];
    ClearTexture(canvas, SKY_COLOR);
    UpdateAndRenderScene(activeScene, canvas, delta);
}
