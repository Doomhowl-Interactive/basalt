#pragma once
#include "basalt.h"

#define BULLET

#define MAX_PATTERNS 128
#define MAX_SPAWNERS 32
#define MAX_ACTIONS 16
#define MAX_BULLET_SLOTS 16
#define MAX_PARAMETERS 16

extern usize GameDifficulty;
#define WIDTH 800
#define HEIGHT 600

#define FLAG_PLAYER (1 << 0)
#define FLAG_BULLET (2 << 0)
#define FLAG_ENEMY  (3 << 0)

typedef uint EntityFlag;
typedef size_t EntityID;

typedef struct Entity Entity;
typedef struct Scene Scene;

// Some generic RAM to put data in
typedef struct ActionData {
    float timer;
    Vec2 origin;
    Vec2 normal;
    float delta;

    float difficulty;
    int ints[MAX_BULLET_SLOTS];
    bool bools[MAX_BULLET_SLOTS];
    float floats[MAX_BULLET_SLOTS];
} ActionData;

typedef void (*BulletActionFunc)(Entity* entity, ActionData* data, const int* args);
typedef bool (*BulletActionEndFunc)(Entity* entity, ActionData* data, const int* args);

typedef struct BulletAction {
    BulletActionFunc function;
    BulletActionEndFunc endFunction;
    Color tint;
    int parameters[MAX_PARAMETERS];
} BulletAction;

typedef bool (*EntityAIActionFunc)(Entity* entity, ActionData* data, const int* args);

typedef struct EntityAIAction {
    EntityAIActionFunc function;
    int parameters[MAX_PARAMETERS];
} EntityAIAction;

typedef struct EntityAI {
    const char* name;
    EntityAIAction actions[MAX_ACTIONS];
    ActionData data;
    usize count;
    usize index;
} EntityAI;

typedef struct BulletPattern {
    const char* name;
    BulletAction actions[MAX_ACTIONS];
    const char* texture;
    ActionData data;
    uint count;
    uint index;

} BulletPattern;

typedef struct BulletSpawner {
    Vec2 offset;
    Vec2 normal;
    float interval;
    float spawnTimer;
    bool disabled;
    const BulletPattern* patternToSpawn;
} BulletSpawner;

// TODO: Use regions for sprites
struct Entity {
    // entity mega struct
    Scene* scene;
    bool isActive;
    float timeAlive;
    EntityID id;
    EntityFlag flags;

    // sprite
    RectF bounds;
    Color tint;
    Point sourceOffset;
    Texture texture;

    float frameInterval;
    float timer;
    int frameID;

    // physics
    Vec2 vel;

    // ship
    float moveSpeed;

    // health
    uint maxHealth;
    uint health;

    // collision
    bool isToucher;
    EntityFlag ignoreFlags;

    // bullet
    BulletPattern bulletPattern;

    // spawner
    BulletSpawner bulletSpawners[MAX_SPAWNERS];

    // ai
    EntityAI ai;
};

#define MAX_ENTITY_PAGES 128
struct Scene {
    Entity* entities[MAX_ENTITY_PAGES];
};

// bullet_entities.c
BULLET void ClearEntities(Scene* scene);
BULLET Entity* CreateEntity(Scene* scene);
BULLET void DestroyEntity(Entity* e);

BULLET void SetEntityCenter(Entity* e, float x, float y);
BULLET Vec2 GetEntityCenter(Entity* e);
BULLET void SetEntitySize(Entity* e, uint width, uint height);
BULLET void ResetEntityVelocity(Entity* e);
BULLET usize GetEntityCount();

BULLET void EntityDamage(Entity* e);
BULLET bool EntityIsActive(Entity* e);
BULLET bool EntityHasFlag(Entity* e, EntityFlag flag);
BULLET Entity* GetFirstEntityWithFlag(Scene* scene, EntityFlag flag);

typedef void (*EntityCallback)(Entity* e, int i);
BULLET usize ForeachSceneEntity(Scene* scene, EntityCallback callback);

BULLET void UpdateAndRenderEntity(Scene* scene, Texture canvas, Entity* e, float delta);
BULLET usize UpdateAndRenderScene(Scene* scene, Texture canvas, float delta);

// bullet_levels.c
#define BACKGROUND
#define SCHEDULE

#define MAX_SCHEDULED_ITEMS 256

struct LevelSchedule;
typedef void (*BackgroundRenderFunc)(Texture canvas, float delta);
typedef void (*LevelSchedulerFunc)(int difficulty);
typedef void (*EntityInitializerFunc)(Entity* e, Vec2 pos);

typedef struct LevelScheduleItem {
    double delayTime;
    EntityInitializerFunc initFunc;
    Vec2 position;
    const char* description;
} LevelScheduleItem;

typedef struct LevelSchedule {
    LevelScheduleItem items[MAX_SCHEDULED_ITEMS];
    usize curIndex;
    usize itemCount;
    double lastScheduleTime;
} LevelSchedule;

typedef struct LevelInfo {
    int number;
    const char* name;
    LevelSchedulerFunc schedulerFunc;
    BackgroundRenderFunc backgroundFunc;
} LevelInfo;

typedef void (*LevelInitializerFunc)(const LevelInfo* level);

extern const LevelInfo Level1;

BULLET void SwitchLevel(const LevelInfo* level);
BULLET bool UpdateAndRenderLevel(Texture canvas, Scene* scene, float delta);
BULLET void RunLevelEnterHook(LevelInitializerFunc initFunc);

// bullet_patterns.c
BULLET bool RunBulletPattern(Entity* e, float delta);
BULLET const BulletPattern* GetBulletPattern(usize index);
BULLET const BulletPattern* GetBulletPatternByName(const char* name);
BULLET usize GetBulletPatternCount();
BULLET void FillInActionData(ActionData* data, float delta);
BULLET extern void ResetActionData(ActionData* data);

// bullet_collision.c
BULLET void CheckCollisionOfEntity(Entity* e, Scene* scene);

// bullet_ai.c
BULLET bool RunEntityAI(Entity* e, float delta);
BULLET const EntityAI* GetEntityAI(usize index);
BULLET const EntityAI* GetEntityAIByName(const char* name);
BULLET usize GetEntityAICount();

// bullet_factories.c
BULLET void InitPlayer(Entity* e, Vec2 pos);
BULLET void InitTestEnemy(Entity* e, Vec2 pos);
BULLET void InitBullet(Entity* e, const BulletPattern* pattern, Vec2 pos, Vec2 normal);

// bullet_gui.c
BULLET void UpdateAndRenderGUI(Texture canvas, Entity* player, float delta);

// bullet_dialog.c
BULLET void ConstructDialogs();

// bullet_editor.c
BULLET bool IsEditorOpen();
BULLET Rect GetEditorTabContentRegion();  // NOTE: Gets a Rectangle of the screen without the toolbar at the top
BULLET void UpdateAndRenderEditor(Scene* activeScene, Texture canvas, float delta);

// bullet_editor_patterns.c
BULLET void UpdateAndRenderPatternEditor(Texture canvas, float delta);

// bullet_tests.c
BULLET void UnitTestBullet();
