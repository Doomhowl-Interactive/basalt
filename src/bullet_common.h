#pragma once
#include "bullet_assets.h"

#define MAX_ENTITIES 1024
#define MAX_SPAWNERS 32
#define MAX_PATTERNS 16
#define MAX_BULLET_SLOTS 16
#define MAX_PARAMETERS 16

#define TAG_PLAYER      (1 << 0)
#define TAG_BULLET      (1 << 1)

#define COMPARE(X,Y) ((X & Y) == Y)
#define BULLET

typedef uint EntityID;
typedef uint EntityType;

typedef struct Entity Entity;
typedef struct Scene Scene;

typedef struct BulletData {
    float timer;
    Vec2 origin;
    Vec2 normal;
    float delta;

    int ints[MAX_BULLET_SLOTS];
    bool bools[MAX_BULLET_SLOTS];
    float floats[MAX_BULLET_SLOTS];
} BulletData;

typedef void (*BulletActionFunc)(Entity* entity, BulletData* data, int difficulty, const int* args);
typedef bool (*BulletActionEndFunc)(Entity* entity, BulletData* data, int difficulty, const int* args);

class(BulletAction) {
    BulletActionFunc function;
    BulletActionEndFunc endFunction;
    Color tint;
    int parameters[MAX_PARAMETERS];
};

class(BulletPattern) {
    BulletAction actions[MAX_PATTERNS];
    Texture* texture;
    uint count;
    uint index;
};

class(BulletSpawner) {
    Vec2 offset;
    Vec2 normal;
    float interval;
    float spawnTimer;
    const BulletPattern* patternToSpawn;
};

// TODO: Use regions for sprites
struct Entity {
    // entity mega struct
    Scene* scene;
    bool isActive;
    float timeAlive;
    EntityID id;
    EntityType type;

    // sprite
    RectF bounds;
    Color tint;
    Point sourceOffset;
    Texture texture;

    // physics
    Vec2 vel;

    // ship
    float moveSpeed;
    
    // health
    uint maxHealth;
    uint health;

    // bullet
    BulletData bulletData;
    BulletPattern bulletPattern;

    // spawner
    BulletSpawner bulletSpawners[MAX_SPAWNERS];
};

struct Scene {
    uint id;
    Entity entities[MAX_ENTITIES];
};

// bullet_entities.c
BULLET void ClearEntities(Scene* scene);
BULLET Entity* CreateEntity(Scene* scene);
BULLET void DestroyEntity(Entity* e);

BULLET void SetEntityCenter(Entity* e, float x, float y);
BULLET Vec2 GetEntityCenter(Entity* e);
BULLET void SetEntitySize(Entity* e, uint width, uint height);
BULLET void ResetEntityVelocity(Entity *e);

BULLET void UpdateAndRenderEntity(Scene* scene, Texture canvas, Entity* e, float delta);
BULLET uint UpdateAndRenderScene(Scene* scene, Texture canvas, float delta);

// bullet_patterns.c
BULLET bool RunBulletPattern(Entity* e, float delta);

// bullet_factories.c
BULLET void InitPlayer(Entity* e, Vec2 pos);
BULLET void InitBullet(Entity* e, const BulletPattern* pattern, Vec2 pos, Vec2 normal);

// bullet_tests.c
platfunc void UnitTestBullet();

extern Texture PlayerTexture;
extern Texture BulletPlacholderTexture;
extern const BulletPattern PlayerBullet;
extern const BulletPattern PlayerBullet2;
extern const BulletPattern PlayerBullet3;
extern const BulletPattern PlayerBullet4;
