#include "basalt.h"
#include "basalt_extra.h"
#include "temple_assets.h"
#include "bullet_common.h"

#include <math.h>
#include <stdio.h>

const uint WIDTH = 800;
const uint HEIGHT = 600;
const char* GAME_TITLE = "Bullethell game";

const uint TPS = 25;

#define SKY_COLOR 0x000000FF
#define SCENE_GAME              0
#define SCENE_BULLET_PREVIEW    1
#define SCENE_COUNT             2

static uint ActiveSceneID     = 0;
static Scene Scenes[SCENE_COUNT] = { 0 };

static Entity* Player = NULL;
static uint SelectedPattern = 1;

static float BackgroundScrollSpeed = 100.f;
static Texture BackgroundNoiseTexture = { 0 };

Texture BulletPlacholderTexture = { 0 };
Texture PlayerTexture = { 0 };

pubfunc void InitializeGame()
{
    if (Config.hasUnitTesting)
        UnitTestBullet();

    BulletPlacholderTexture = RequestTexture(SPR_BULLET_PLACEHOLDER);
    BackgroundNoiseTexture = RequestTexture(SPR_BACKGROUND_NOISE_COL);
    PlayerTexture = RequestTexture(SPR_SHIP_PLAYER);

    Player = CreateEntity(&Scenes[SCENE_GAME]);
    Vec2 spawnPos = {WIDTH / 2, HEIGHT / 1.2f};
    InitPlayer(Player, spawnPos);
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

    // switch between bullet types for testing
    if (IsKeyPressed(KEY_E))
    {
        uint patternIndex = SelectedPattern++;
        for (int i = 0; i < MAX_SPAWNERS; i++)
        {
            BulletSpawner* spawner = &Player->bulletSpawners[i];
            if (spawner == NULL) continue;
            spawner->patternToSpawn = GetBulletPattern(patternIndex);
        }

        if (SelectedPattern >= GetBulletPatternCount())
            SelectedPattern = 0;
    }

    // TODO: blend at runtime 0x4B486EFF, 0x07060FFF
    int offsetY = (int)(GetTimeElapsed()*-BackgroundScrollSpeed) % HEIGHT;
    DrawTexture(canvas, BackgroundNoiseTexture, 0.f, -offsetY, WHITE);
    DrawTexture(canvas, BackgroundNoiseTexture, 0.f, -offsetY+HEIGHT, WHITE);

    Scene* activeScene = &Scenes[ActiveSceneID];
    UpdateAndRenderScene(activeScene, canvas, delta);
    UpdateAndRenderEditor(activeScene, canvas, delta);

    // draw fps
    static char info[128];
    float fps = 1.f / delta;
    sprintf(info, "FPS %1.3f", fps);
    DrawText(canvas, info, 10, HEIGHT-20, fps < 30 ? RED:GREEN);
}
