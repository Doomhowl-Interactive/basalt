#include "basalt.h"
#include "bullet_common.h"
#define MAX_INITIALIZERS 64

typedef struct LevelLEVEL {
    double timePassed;
    LevelSchedule schedule;

    const LevelInfo* currentLevel;

    LevelInitializerFunc initializers[MAX_INITIALIZERS];
    usize initializerCount;
} LevelLEVEL;

static LevelLEVEL LEVEL = { 0 };

// ==== API functions ==== //
BULLET void SwitchLevel(const LevelInfo* level)
{
    // Run level scheduler
    level->schedulerFunc(GameDifficulty);
    LEVEL.currentLevel = level;

    INFO("Switched to level %s", level->name);

    // Call subscribers
    for (usize i = 0; i < LEVEL.initializerCount; i++) {
        LevelInitializerFunc initFunc = LEVEL.initializers[i];
        if (initFunc) {
            (*initFunc)(level);
        }
    }
}

BULLET void RunLevelEnterHook(LevelInitializerFunc initFunc)
{
    // check if present
    for (usize i = 0; i < LEVEL.initializerCount; i++) {
        if (LEVEL.initializers[i] == initFunc) {
            return;  // already added
        }
    }

    // make subscriber
    if (LEVEL.initializerCount < MAX_INITIALIZERS) {
        LEVEL.initializers[LEVEL.initializerCount++] = initFunc;
    } else {
        ERR("Too many level initializers defined!");
    }

    // send level immediately
    (*initFunc)(LEVEL.currentLevel);
}

func void UpdateAndRenderBackground(Texture canvas, float delta);
func void RunScheduler(LevelSchedule* schedule, Scene* scene);
BULLET bool UpdateAndRenderLevel(Texture canvas, Scene* scene, float delta)
{
    RunScheduler(&LEVEL.schedule, scene);
    UpdateAndRenderBackground(canvas, delta);
    UpdateAndRenderScene(scene, canvas, delta);
    UpdateAndRenderEditor(scene, canvas, delta);

    Entity* player = GetFirstEntityWithFlag(scene, FLAG_PLAYER);
    if (player != NULL){
        UpdateAndRenderGUI(canvas, player, delta);
    }

    return false;
}

// ==== API local functions ==== //
func void UpdateAndRenderBackground(Texture canvas, float delta)
{
    if (Config.lowQuality)
    {
        ClearTexture(canvas, 0x0);
    }
    else
    {
        assert(LEVEL.currentLevel);
        if (LEVEL.currentLevel->backgroundFunc)
        {
            LEVEL.currentLevel->backgroundFunc(canvas, delta);
        }
    }

}

// ==== Level scheduling ==== //
func double TotalDelayUntilScheduledItem(LevelSchedule* schedule, LevelScheduleItem* item)
{
    double totalTime = 0.0;
    for (int i = 0; i < MAX_SCHEDULED_ITEMS; i++) {
        LevelScheduleItem* it = &schedule->items[i];
        if (it == item) {
            break;
        }
        totalTime += it->delayTime;
    }
    return totalTime;
}

func void RunScheduler(LevelSchedule* schedule, Scene* scene)
{
    if (schedule->itemCount == schedule->curIndex) {
        return;
    }

    const LevelScheduleItem item = schedule->items[schedule->curIndex];
    if (GetTimeElapsed() > schedule->lastScheduleTime + item.delayTime) {
        assert(item.initFunc);
        schedule->lastScheduleTime = GetTimeElapsed();

        // Spawn scheduled entity
        Entity* e = CreateEntity(scene);
        item.initFunc(e, item.position);

        schedule->curIndex++;
    }
}

func void ScheduleEntityEx(double delay, float x, float y, EntityInitializerFunc initFunc, const char* desc)
{
    LevelSchedule* schedule = &LEVEL.schedule;
    LevelScheduleItem* item = &schedule->items[schedule->itemCount++];
    item->delayTime = delay;
    item->initFunc = initFunc;
    item->position = (Vec2){ x, y };
    item->description = desc;

    double totalDelay = TotalDelayUntilScheduledItem(schedule, item);
    DEBUG("Scheduled entity at %f seconds", totalDelay);
}

func inline void ScheduleEntity(double delay, float x, float y, EntityInitializerFunc initFunc)
{
    ScheduleEntityEx(delay, x, y, initFunc, "Unnamed");
}

func void ScheduleEntityColumns(double delay, EntityInitializerFunc initFunc, uint count)
{
    int segWidth = WIDTH / count;
    for (int i = 0; i < count; i++) {
        int x = i * segWidth;
        Vec2 spawnPos = { x, -100 };
        ScheduleEntity(delay, V2(spawnPos), initFunc);
    }
}

SCHEDULE void TrainingLevelLayout(int difficulty)
{
    ScheduleEntity(0.0, WIDTH / 2, -100, InitTestEnemy);
    ScheduleEntity(1.0, WIDTH / 4, -100, InitTestEnemy);
    ScheduleEntity(0.0, WIDTH - WIDTH / 4, -100, InitTestEnemy);
    ScheduleEntityColumns(5, InitTestEnemy, 10);
}

// ==== Backgroud rendering ==== //
BACKGROUND void ScrollingPurpleNoise(Texture canvas, float delta)
{
    // TODO: blend at runtime 0x4B486EFF, 0x07060FFF
    const float backgroundScrollSpeed = 100.f;
    Texture noiseTexture = RequestTexture("SPR_BACKGROUND_NOISE_COL");
    int offsetY = (int)(GetTimeElapsed() * backgroundScrollSpeed) % HEIGHT;
    DrawTexture(canvas, noiseTexture, 0.f, -offsetY, WHITE);
    DrawTexture(canvas, noiseTexture, 0.f, -offsetY + HEIGHT, WHITE);
}

// ==== Level definitions ==== //
// TODO: Use indices instead
const LevelInfo Level1 = {
    0,
    "Training Ground",
    TrainingLevelLayout,
    ScrollingPurpleNoise,
};
