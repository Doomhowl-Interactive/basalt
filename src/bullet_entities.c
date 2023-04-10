#include "basalt_extra.h"
#include "bullet_common.h"

#include <string.h>

// TODO: Use regions for sprites instead of top-left pos

BULLET void ClearEntities(Scene* scene)
{
    assert(scene);

    memset(scene->entities, 0, sizeof(Entity)*MAX_ENTITIES);
    INFO("Cleared all entities!");
}

BULLET Entity* CreateEntity(Scene* scene)
{
    assert(scene);

    // get next available entity
    for (uint i = 0; i < MAX_ENTITIES; i++)
    {
        Entity* entity = &scene->entities[i];
        if (!entity->isActive)
        {
            entity->id = i;
            entity->scene = scene;
            entity->isActive = true;
            return entity;
        }
    }

    assert(0);
    return NULL;
}

BULLET void DestroyEntity(Entity* e)
{
    memset(e, 0, sizeof(Entity));
}

BULLET void SetEntityCenter(Entity* e, float x, float y)
{
    e->sprite.bounds.x = x - e->sprite.bounds.width * 0.5f;
    e->sprite.bounds.y = y - e->sprite.bounds.height * 0.5f;
}

BULLET Vec2 GetEntityCenter(Entity* e)
{
    return RectFCenter(e->sprite.bounds);
}

BULLET void SetEntitySize(Entity* e, uint width, uint height)
{
    float posX = e->sprite.bounds.x;
    float posY = e->sprite.bounds.y;

    e->sprite.bounds.width = width;
    e->sprite.bounds.height = height;

    SetEntityCenter(e, posX, posY);
}

BULLET void ResetEntityVelocity(Entity *e)
{
    e->physics.vel.x = 0;
    e->physics.vel.y = 0;
}

void UpdateAndRenderEntity(Scene* scene, Texture canvas, Entity* e, float delta)
{
    e->timeAlive += delta;

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
        if (e->sprite.texture.pixels)
            DrawTextureV(canvas, e->sprite.texture, RectFOrigin(e->sprite.bounds));
        else
            DrawRectangleRec(canvas, RectFToRect(e->sprite.bounds), e->sprite.tint);
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

    // WEAPON BEHAVIOUR
    for (uint i = 0; i < MAX_SPAWNERS; i++)
    {
        Entity* weapon = e->weapon.spawners[i];
        if (weapon == NULL) continue;

        // set weapon into correct position
        Vec2 center = RectFCenter(e->sprite.bounds);
        center = Vec2Offset(center, weapon->spawner.offsetFromParent);
        SetEntityCenter(e, center.x, center.y);

        // draw normal (debugging)
        Vec2 weaponCenter = GetEntityCenter(weapon);
        Vec2 end = Vec2Offset(weaponCenter, Vec2Scale(weapon->spawner.normal, 10.f)); 
        DrawLineV(canvas, weaponCenter, end, 0x0000AAFF);

        // spawn bullets on interval
        if (weapon->spawner.spawnTimer > weapon->spawner.interval)
        {
            Entity* bul = CreateEntity(scene);
            InitBullet(bul, weapon->spawner.patternToSpawn, weaponCenter, weapon->spawner.normal);
            weapon->spawner.spawnTimer = 0.f;
        }
        weapon->spawner.spawnTimer += delta;
    }

    // Bullet behaviour
    if (COMPARE(e->type,TAG_BULLET))
    {
        if (RunBulletPattern(e, delta))
            DestroyEntity(e);
    }

    // apply movement
    e->sprite.bounds.x += vel->x*delta;
    e->sprite.bounds.y += vel->y*delta;
    
    // apply drag
    // float offsetX = e->physics.drag * delta * SIGN(float, vel->x);
    // vel->x -= MIN(offsetX, vel->x);
    // float offsetY = e->physics.drag * delta * SIGN(float, vel->y);
    // vel->y -= MIN(offsetY, vel->y);
}

uint UpdateAndRenderScene(Scene* scene, Texture canvas, float delta)
{
    uint count = 0;
    for (uint i = 0; i < MAX_ENTITIES; i++)
    {
        Entity* e = &scene->entities[i];
        if (e->isActive)
        {
            UpdateAndRenderEntity(scene, canvas, e, delta);
            count++;
        }
    }
    return count;
}
