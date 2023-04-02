#include "basalt.h"
#include "basalt_extra.h"

#define MAX_ENTITIES 256

#define ENTITY_PLAYER           (1 << 0)

#define COMPARE(X,Y) ((X & Y) == Y)

typedef uint EntityID;
typedef uint EntityType;

typedef enum BulletType {
    Default,
} BulletType;

// TODO: Use anonymous structs
// entity mega struct
typedef struct Entity Entity;
struct Entity {
    EntityID id;
    EntityType type;

    // Sprite
    Color tint;
    Rect source;
    Texture texture;

    // Moveable 
    Vec2 vel;
    float drag;

    // Player
    Vec2 pos;
    float moveSpeed;
    
    // Health
    uint maxHealth;
    uint health;

    // Bullet spawner
    float interval;
    BulletType bulletType;
    uint bulletDamage;

    // Bullet spawner container
    Entity* spawners;
    uint spawnerCount;

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
        if (entity->type == 0)
        {
            entity->id = i;
            return entity; 
        }
    }

    assert(0);
    return NULL;
}

void SetEntitySize(Entity* e, uint width, uint height)
{
    e->source.width = width;
    e->source.height = height;
    e->texture.width = width;
    e->texture.height = height;
}

void InitPlayer(Entity* e, Vec2 pos)
{
    e->pos = (Vec2) { pos.x - 48 / 2, pos.y };
    e->type = ENTITY_PLAYER;
    e->tint = 0x00FF00FF;
    e->moveSpeed = 200;
    SetEntitySize(e, 32, 32);
}

Rect GetEntityBounds(Entity entity)
{
    Rect bounds = {
        entity.pos.x,
        entity.pos.y,
        entity.source.width,
        entity.source.height
    };
    return bounds;
}

void UpdateAndRenderEntity(Texture canvas, Entity* e, float delta)
{
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

    if (e->texture.width > 0)
    {
        Rect bounds = GetEntityBounds(*e);
        DrawRectangleRec(canvas, bounds, e->tint);

    }

    // Player behaviour
    if (COMPARE(e->type,ENTITY_PLAYER))
    {
        e->vel.x = 0;
        e->vel.y = 0;
        if (IsKeyDown(KEY_A))
        {
            e->vel.x -= e->moveSpeed;
        }
        if (IsKeyDown(KEY_D))
        {
            e->vel.x += e->moveSpeed;
        }
        if (IsKeyDown(KEY_W))
        {
            e->vel.y -= e->moveSpeed;
        }
        if (IsKeyDown(KEY_S))
        {
            e->vel.y += e->moveSpeed;
        }
    }

    // apply movement
    e->pos.x += e->vel.x*delta;
    e->pos.y += e->vel.y*delta;
    
    // apply drag
    float offsetX = e->drag * delta * SIGN(float, e->vel.x);
    e->vel.x -= MIN(offsetX, e->vel.x);
    float offsetY = e->drag * delta * SIGN(float, e->vel.y);
    e->vel.y -= MIN(offsetY, e->vel.y);
}

uint UpdateAndRenderEntities(Texture canvas, float delta)
{
    uint count = 0;
    for (uint i = 0; i < MAX_ENTITIES; i++)
    {
        Entity* e = &GameEntities[i];
        if (e->type > 0)
        {
            UpdateAndRenderEntity(canvas, e, delta);
            count++;
        }
    }
    return count;
}