#include "basalt.h"
#include "basalt_extra.h"
#include "bullet_common.h"

#include <math.h>
#include <stdio.h>

#define SKY_COLOR 0x000000FF
#define SCENE_GAME 0
#define SCENE_BULLET_PREVIEW 1
#define SCENE_COUNT 2

static uint ActiveSceneID = 0;
static Scene Scenes[SCENE_COUNT] = { 0 };

static Entity* Player = NULL;
static uint SelectedPattern = 1;

static float BackgroundScrollSpeed = 100.f;
static Texture BackgroundNoiseTexture = { 0 };

#define TEST_ENEMY_COUNT 10
static Entity* TestEnemies[TEST_ENEMY_COUNT];

Texture BulletPlacholderTexture = { 0 };
Texture PlayerTexture = { 0 };

usize GameDifficulty = 2;

DYNAMIC BASALT GameConfig ConfigureGame()
{
    GameConfig config;
    config.title = GetRealRandomNumber() % 1000 == 699 ? "Guardians Of Ur Mum" : "Guardians Of The Cursed Fire";
    config.width = WIDTH;
    config.height = HEIGHT;
    config.language = 1;
    return config;
}

DYNAMIC BASALT void InitializeGame()
{
    if (Config.hasUnitTesting)
        UnitTestBullet();

    BulletPlacholderTexture = RequestTexture(SPR_BULLET_PLACEHOLDER);
    BackgroundNoiseTexture = RequestTexture(SPR_BACKGROUND_NOISE_COL);
    PlayerTexture = RequestTexture(SPR_SHIP_PLAYER);

    Player = CreateEntity(&Scenes[SCENE_GAME]);
    Vec2 spawnPos = { WIDTH / 2, HEIGHT / 1.2f };
    InitPlayer(Player, spawnPos);

    for (int i = 0; i < TEST_ENEMY_COUNT; i++) {
        Entity* enemy = CreateEntity(&Scenes[ActiveSceneID]);
        InitTestEnemy(enemy, (Vec2){ 50, 50 });
        TestEnemies[i] = enemy;
    }
}

DYNAMIC BASALT void DisposeGame()
{
    WARN("TODO: dispose game properly");
}

DYNAMIC BASALT void UpdateAndRenderGame(Texture canvas, float delta)
{
    if (IsKeyPressed(KEY_I)) {
        ActiveSceneID++;
        if (ActiveSceneID >= SCENE_COUNT)
            ActiveSceneID = 0;
    }
    if (IsKeyPressed(KEY_U)) {
        ActiveSceneID--;
        if (ActiveSceneID < 0)
            ActiveSceneID = SCENE_COUNT - 1;
    }

    if (IsKeyPressed(KEY_M)) {
        TakeScreenshot(canvas);
    }

    // TODO: blend at runtime 0x4B486EFF, 0x07060FFF
    int offsetY = (int)(GetTimeElapsed() * BackgroundScrollSpeed) % HEIGHT;
    DrawTexture(canvas, BackgroundNoiseTexture, 0.f, -offsetY, WHITE);
    DrawTexture(canvas, BackgroundNoiseTexture, 0.f, -offsetY + HEIGHT, WHITE);

    Scene* activeScene = &Scenes[ActiveSceneID];
    UpdateAndRenderScene(activeScene, canvas, delta);
    UpdateAndRenderEditor(activeScene, canvas, delta);

    // draw fps
    static char info[128];
    float fps = 1.f / delta;
    sprintf(info, "FPS %1.3f", fps);
    DrawText(canvas, info, 10, HEIGHT - 20, fps < 30 ? RED : GREEN);

    // draw the name of the pattern being shot
    const char* name = Player->bulletSpawners[0].patternToSpawn->name;
    DrawText(canvas, name, 250, HEIGHT - 60, 0xFF0000FF);
}
