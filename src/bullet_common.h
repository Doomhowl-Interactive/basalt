#pragma once

#define MAX_ENTITIES 256
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

typedef bool (*BulletActionFunc)(Entity* entity, BulletData* data, int difficulty, const int* args);

typedef struct BulletAction {
    BulletActionFunc function;
    Color tint;
    int parameters[MAX_PARAMETERS];
} BulletAction;

typedef struct BulletPattern {
    BulletAction actions[MAX_PATTERNS];
    uint count;
} BulletPattern;

// TODO: Use regions for sprites
struct Entity {
    // entity mega struct
    Scene* scene;
    bool isActive;
    float timeAlive;
    EntityID id;
    EntityType type;

    struct {
        Vec2 pos;
        Color tint;
        Rect source;
        Texture texture;
    } sprite;

    struct {
        Vec2 vel;
        float drag;
    } physics;

    struct {
        float moveSpeed;
    } ship;
    
    struct {
        uint maxHealth;
        uint health;
    } alive;

    struct {
        BulletData data;
        BulletPattern pattern;
        uint curPatternIndex;
    } bullet;

    struct {
        Vec2 offsetFromParent;
        Vec2 normal;
        float interval;
        float spawnTimer;
        const BulletPattern* patternToSpawn;
    } spawner;

    struct {
        Entity* spawners[MAX_SPAWNERS];
    } weapon;
};

struct Scene {
    uint id;
    Entity entities[MAX_ENTITIES];
};

// bullet_entities.c
BULLET void ClearEntities(Scene* scene);
BULLET Entity* CreateEntity(Scene* scene);
BULLET void DestroyEntity(Entity* e);

BULLET void SetEntitySize(Entity* e, uint width, uint height);
BULLET Rect GetEntityBounds(Entity e);
BULLET void ResetEntityVelocity(Entity *e);

BULLET void UpdateAndRenderEntity(Scene* scene, Texture canvas, Entity* e, float delta);
BULLET uint UpdateAndRenderScene(Scene* scene, Texture canvas, float delta);

BULLET void InitPlayer(Entity* e, Vec2 pos);
BULLET void InitBullet(Entity* e, const BulletPattern* patterns, Vec2 pos, Vec2 normal);

// bullet_patterns.c
BULLET bool RunBulletPattern(Entity* e, float delta);

extern const BulletPattern PlayerBullet;
extern const BulletPattern PlayerBullet2;
extern const BulletPattern PlayerBullet3;
