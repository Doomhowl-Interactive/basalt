#include "basalt.h"
#include "basalt_extra.h"
#include "temple_assets.h"

#include <math.h>
#include <stdio.h>

const uint WIDTH = 800;
const uint HEIGHT = 600;
const char* GAME_TITLE = "Bullethell game";

const uint TPS = 25;

#define SKY_COLOR 0x000000FF

#include "bullet_entities.c"
#include "bullet_patterns.c"

#define SCENE_GAME              0
#define SCENE_BULLET_PREVIEW    1
#define SCENE_COUNT             2

static uint ActiveSceneID     = 0;
static Scene Scenes[SCENE_COUNT] = { 0 };

static Entity* Player = NULL;

#define PATTERN_COUNT 4
static const BulletPattern* Patterns[] = {
    &PlayerBullet,
    &PlayerBullet2,
    &PlayerBullet3,
    &PlayerBullet4,
};
static uint SelectedPattern = 1;
static float BackgroundScrollSpeed = 100.f;

Texture BulletPlacholderTexture = { 0 };

pubfunc void InitializeGame()
{
    Player = CreateEntity(&Scenes[SCENE_GAME]);
    Vec2 spawnPos = {WIDTH / 2, HEIGHT / 1.2f};
    InitPlayer(Player, spawnPos);

    BulletPlacholderTexture = LoadTexture(SPR_BULLET_PLACEHOLDER);
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
            Entity* weapon = Player->weapon.spawners[i];
            if (weapon == NULL) continue;
            weapon->spawner.patternToSpawn = Patterns[patternIndex];
        }

        if (SelectedPattern >= PATTERN_COUNT)
            SelectedPattern = 0;
    }

    Rect window = { 0, 0, WIDTH, HEIGHT };
    Vec2 offset = { 0.f, GetTimeElapsed()*BackgroundScrollSpeed };
    DrawNoiseRectangle(canvas, window, offset, 0x4B486EFF, 0x07060FFF);

    Scene* activeScene = &Scenes[ActiveSceneID];
    UpdateAndRenderScene(activeScene, canvas, delta);
}
