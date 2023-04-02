#include "basalt.h"
#include "basalt_extra.h"

#define MAX_ENTITIES 256
#define MAX_SPAWNERS 32

#define TAG_PLAYER      (1 << 0)

#define COMPARE(X,Y) ((X & Y) == Y)

typedef uint EntityID;
typedef uint EntityType;

typedef enum BulletType {
    Default,
} BulletType;

// entity mega struct
typedef struct Entity Entity;
struct Entity {
    bool isActive;
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
        Vec2 offsetFromParent;
        Vec2 normal;
        float interval;
        BulletType bulletType;
    } spawner;

    struct {
        Entity* spawners[MAX_SPAWNERS];
    } weapon;
};

static Entity GameEntities[MAX_ENTITIES];

void ClearEntities()
{
    memset(GameEntities, 0, sizeof(Entity)*MAX_ENTITIES);
    INFO("Cleared all entities!");
}

Entity* CreateEntity()
{
    // get next available entity
    for (uint i = 0; i < MAX_ENTITIES; i++)
    {
        Entity* entity = &GameEntities[i];
        if (!entity->isActive)
        {
            entity->id = i;
            entity->isActive = true;
            return entity; 
        }
    }

    assert(0);
    return NULL;
}

void SetEntitySize(Entity* e, uint width, uint height)
{
    e->sprite.source.width = width;
    e->sprite.source.height = height;
    e->sprite.texture.width = width;
    e->sprite.texture.height = height;
}

void InitPlayer(Entity* e, Vec2 pos)
{
    e->type = TAG_PLAYER;
    e->sprite.pos = (Vec2) { pos.x - 48 / 2, pos.y };
    e->sprite.tint = 0x00FF00FF;
    e->ship.moveSpeed = 200;
    SetEntitySize(e, 32, 32);

    // Bullet spawners
    double outwardsAngleDeg = 70;
    double distanceFromPlayer = 45;
    uint spawnerCount = 5;

    double anglePerSpawner = outwardsAngleDeg / spawnerCount;
    for (uint i = 0; i < spawnerCount; i++)
    {
        double angle = -90 - outwardsAngleDeg * 0.5f + anglePerSpawner * i + anglePerSpawner * 0.5f;

        Entity* ent = CreateEntity();
        ent->sprite.tint = 0xFFFF00FF;
        ent->spawner.normal.x = cos(DEG2RAD(angle));
        ent->spawner.normal.y = sin(DEG2RAD(angle));
        ent->spawner.offsetFromParent.x = ent->spawner.normal.x*distanceFromPlayer;
        ent->spawner.offsetFromParent.y = ent->spawner.normal.y*distanceFromPlayer;
        SetEntitySize(ent, 2,2);

        e->weapon.spawners[i] = ent;
    }
}

Rect GetEntityBounds(Entity e)
{
    Rect bounds = {
        e.sprite.pos.x,
        e.sprite.pos.y,
        e.sprite.source.width,
        e.sprite.source.height
    };
    return bounds;
}

void UpdateAndRenderEntity(Texture canvas, Entity* e, float delta)
{
    // Shortcuts
    Vec2* vel = &e->physics.vel;

    // Entity drawing
    // TODO: Put in entity struct
    static float frameInterval = 0.2f;
    static float timer = 0.f;
    static int frameID = 0;

    if (timer > frameInterval)
    {
        timer = 0.f;
        frameID++;
    }
    timer += delta;

    if (e->sprite.texture.width > 0)
    {
        Rect bounds = GetEntityBounds(*e);
        DrawRectangleRec(canvas, bounds, e->sprite.tint);
    }

    // Player behaviour
    if (COMPARE(e->type,TAG_PLAYER))
    {
        float moveSpeed = e->ship.moveSpeed;
        vel->x = 0;
        vel->y = 0;

        if (IsKeyDown(KEY_A))
        {
            vel->x -= moveSpeed;
        }
        if (IsKeyDown(KEY_D))
        {
            vel->x += moveSpeed;
        }
        if (IsKeyDown(KEY_W))
        {
            vel->y -= moveSpeed;
        }
        if (IsKeyDown(KEY_S))
        {
            vel->y += moveSpeed;
        }
    }

    // TODO: rename spawners to weapon
    // spawner container behaviour
    for (uint i = 0; i < MAX_SPAWNERS; i++)
    {
        Entity* spawner = e->weapon.spawners[i];
        if (spawner == NULL) continue;

        Rect bounds = GetEntityBounds(*e);
        float centerX = bounds.x + bounds.width * 0.5f;
        float centerY = bounds.y + bounds.height * 0.5f;
        spawner->sprite.pos.x = centerX + spawner->spawner.offsetFromParent.x;
        spawner->sprite.pos.y = centerY + spawner->spawner.offsetFromParent.y;

        // draw normal
        Vec2 end;
        end.x = spawner->sprite.pos.x + spawner->spawner.normal.x * 10;
        end.y = spawner->sprite.pos.y + spawner->spawner.normal.y * 10;
        DrawLineV(canvas, spawner->sprite.pos, end, 0x0000AAFF);
    }
    
    // apply movement
    e->sprite.pos.x += vel->x*delta;
    e->sprite.pos.y += vel->y*delta;
    
    // apply drag
    float offsetX = e->physics.drag * delta * SIGN(float, vel->x);
    vel->x -= MIN(offsetX, vel->x);
    float offsetY = e->physics.drag * delta * SIGN(float, vel->y);
    vel->y -= MIN(offsetY, vel->y);
}

uint UpdateAndRenderEntities(Texture canvas, float delta)
{
    uint count = 0;
    for (uint i = 0; i < MAX_ENTITIES; i++)
    {
        Entity* e = &GameEntities[i];
        if (e->isActive)
        {
            UpdateAndRenderEntity(canvas, e, delta);
            count++;
        }
    }
    return count;
}
