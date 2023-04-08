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

BULLET void SetEntitySize(Entity* e, uint width, uint height)
{
    e->sprite.texture.width = width;
    e->sprite.texture.height = height;
}

BULLET Rect GetEntityBounds(Entity e)
{
    Rect bounds = {
        e.sprite.pos.x,
        e.sprite.pos.y,
        e.sprite.source.width == 0 ? e.sprite.texture.width:e.sprite.source.width,
        e.sprite.source.height == 0 ? e.sprite.texture.height:e.sprite.source.height
    };
    return bounds;
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
        {
            DrawTextureV(canvas, e->sprite.texture, e->sprite.pos);
        }
        else
        {
            Rect bounds = GetEntityBounds(*e);
            DrawRectangleRec(canvas, bounds, e->sprite.tint);
        }
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
        Rect bounds = GetEntityBounds(*e);
        float centerX = bounds.x + bounds.width * 0.5f;
        float centerY = bounds.y + bounds.height * 0.5f;
        weapon->sprite.pos.x = centerX + weapon->spawner.offsetFromParent.x;
        weapon->sprite.pos.y = centerY + weapon->spawner.offsetFromParent.y;

        // draw normal (debugging)
#if 1
        Vec2 end;
        end.x = weapon->sprite.pos.x + weapon->spawner.normal.x * 10;
        end.y = weapon->sprite.pos.y + weapon->spawner.normal.y * 10;
        DrawLineV(canvas, weapon->sprite.pos, end, 0x0000AAFF);
#endif

        // spawn bullets on interval
        if (weapon->spawner.spawnTimer > weapon->spawner.interval)
        {
            Entity* bul = CreateEntity(scene);
            InitBullet(bul, weapon->spawner.patternToSpawn, weapon->sprite.pos, weapon->spawner.normal);
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
    e->sprite.pos.x += vel->x*delta;
    e->sprite.pos.y += vel->y*delta;
    
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

BULLET void InitPlayer(Entity* e, Vec2 pos)
{
    e->type = TAG_PLAYER;
    e->sprite.pos = (Vec2) { pos.x - 48 / 2, pos.y };
    e->sprite.tint = 0x00FF00FF;
    e->ship.moveSpeed = 200;
    SetEntitySize(e, 32, 32);

    // Bullet spawners
    double outwardsAngleDeg = 40;
    double distanceFromPlayer = 45;
    uint spawnerCount = 5;

    double anglePerSpawner = outwardsAngleDeg / spawnerCount;
    for (uint i = 0; i < spawnerCount; i++)
    {
        double angle = -90 - outwardsAngleDeg * 0.5f + anglePerSpawner * i + anglePerSpawner * 0.5f;

        Entity* ent = CreateEntity(e->scene);
        ent->sprite.tint = 0xFFFF00FF;
        // TODO: wrap in builder
        ent->spawner.interval = 0.1f;
        ent->spawner.normal.x = cos(DEG2RAD(angle));
        ent->spawner.normal.y = sin(DEG2RAD(angle));
        ent->spawner.offsetFromParent.x = ent->spawner.normal.x*distanceFromPlayer;
        ent->spawner.offsetFromParent.y = ent->spawner.normal.y*distanceFromPlayer;
        ent->spawner.patternToSpawn = &PlayerBullet;
        SetEntitySize(ent, 2,2);

        e->weapon.spawners[i] = ent;
    }
}

BULLET void InitBullet(Entity* e, const BulletPattern* pattern, Vec2 pos, Vec2 normal)
{
    assert(pattern);

    // set sprite
    e->type = TAG_BULLET;
    e->sprite.pos.x = pos.x;
    e->sprite.pos.y = pos.y;
    e->sprite.source.x = 0;
    e->sprite.source.y = 0;
    e->sprite.source.width = pattern->texture->width;
    e->sprite.source.height = pattern->texture->height;
    e->sprite.texture = *pattern->texture;

    // copy bullet pattern
    e->bullet.pattern = *pattern;
    e->bullet.data.origin = pos;
    e->bullet.data.normal = normal;
    e->sprite.tint = 0xFFFFFFFF;
}
