#include "basalt.h"
#include "basalt_extra.h"
#include "bullet_common.h"

#include <stdlib.h>
#include <string.h>

BULLET void ClearEntities(Scene* scene)
{
    assert(scene);

    memset(scene->entities, 0, sizeof(Entity) * MAX_ENTITIES);
    INFO("Cleared all entities!");
}

func void InitEntity(Entity* entity, Scene* scene)
{
    static usize nextID = 0;
    entity->id = nextID++;
    entity->scene = scene;
    entity->isActive = true;
}

#define ENTITIES_PER_PAGE 1024
func void FreeEmptyPages(Scene* scene)
{
    for (usize i = MAX_ENTITY_PAGES - 1; i > 0; i--) {
        if (scene->entities[i] != NULL) {
            bool isEmpty = true;
            for (usize j = 0; j < ENTITIES_PER_PAGE; j++) {
                if (scene->entities[i][j].isActive) {
                    isEmpty = false;
                    break;
                }
            }
            if (isEmpty) {
                free(scene->entities[i]);
                DEBUG("Freed entity page");
            } else {
                break;
            }
        }
    }
}

BULLET Entity* CreateEntity(Scene* scene)
{
    assert(scene);

    // FreeEmptyPages(scene);

    for (usize i = 0; i < MAX_ENTITY_PAGES; i++) {
        // Allocate the page if it doesn't exist
        if (scene->entities[i] == NULL) {
            scene->entities[i] = calloc(ENTITIES_PER_PAGE, sizeof(Entity));
            assert(scene->entities[i]);
            DEBUG("Allocated new entity page");
        }

        // Find first available slot in this page
        for (usize j = 0; j < ENTITIES_PER_PAGE; j++) {
            assert(scene->entities[i]);
            Entity* entity = &scene->entities[i][j];
            assert(entity);
            if (!entity->isActive) {
                InitEntity(entity, scene);
                return entity;
            }
        }
    }
    ERR("Ran out of pages to store entities in.");
    assert(0);
    return NULL;
}

BULLET void DestroyEntity(Entity* e)
{
    memset(e, 0, sizeof(Entity));
}

BULLET void SetEntityCenter(Entity* e, float x, float y)
{
    e->bounds.x = x - e->bounds.width * 0.5f;
    e->bounds.y = y - e->bounds.height * 0.5f;
}

BULLET Vec2 GetEntityCenter(Entity* e)
{
    return RectFCenter(e->bounds);
}

BULLET void SetEntitySize(Entity* e, uint width, uint height)
{
    Vec2 center = GetEntityCenter(e);
    e->bounds.x = center.x - width * 0.5f;
    e->bounds.y = center.y - height * 0.5f;
    e->bounds.width = width;
    e->bounds.height = height;
}

BULLET void ResetEntityVelocity(Entity* e)
{
    e->vel.x = 0;
    e->vel.y = 0;
}

void UpdateAndRenderEntity(Scene* scene, Texture canvas, Entity* e, float delta)
{
    e->timeAlive += delta;

    // Shortcuts
    Vec2* vel = &e->vel;

    // Entity drawing
    // TODO: Put in entity struct
    static float frameInterval = 0.2f;
    static float timer = 0.f;
    static int frameID = 0;

    if (timer > frameInterval) {
        timer = 0.f;
        frameID++;
    }
    timer += delta;

    if (e->texture.width > 0) {
        if (e->texture.pixels)
            DrawTextureEx(canvas, e->texture, V2(e->bounds), 0, 0, e->bounds.width, e->bounds.height, e->tint);
        else
            DrawRectangle(canvas, R2(e->bounds), e->tint);
    }

    // PLAYER BEHAVIOUR
    if (COMPARE(e->type, TAG_PLAYER)) {
        float moveSpeed = e->moveSpeed;
        vel->x = 0;
        vel->y = 0;

        if (IsKeyDown(KEY_A))
            vel->x -= moveSpeed;
        if (IsKeyDown(KEY_D))
            vel->x += moveSpeed;
        if (IsKeyDown(KEY_W))
            vel->y -= moveSpeed;
        if (IsKeyDown(KEY_S))
            vel->y += moveSpeed;

        // INFO("%f %f %f %f (%f %f)", e->sprite.bounds.x, e->sprite.bounds.y,
        //                             e->sprite.bounds.width, e->sprite.bounds.height, vel->x, vel->y);
    }

    Vec2 center = RectFCenter(e->bounds);

    // WEAPON BEHAVIOUR
    for (uint i = 0; i < MAX_SPAWNERS; i++) {
        BulletSpawner* weapon = &e->bulletSpawners[i];
        if (weapon->patternToSpawn == NULL)
            continue;

        // draw normal (debugging)
        Vec2 weaponCenter = Vec2Offset(center, weapon->offset);
        Vec2 end = Vec2Offset(weaponCenter, Vec2Scale(weapon->normal, 10.f));
        DrawLine(canvas, V2(weaponCenter), V2(end), 0x0000AAFF);

        // spawn bullets on interval
        // HACK: Avoid entity overload
        if (weapon->interval > 0.f && weapon->spawnTimer > weapon->interval) {
            Entity* bul = CreateEntity(scene);
            InitBullet(bul, weapon->patternToSpawn, weaponCenter, weapon->normal);
            weapon->spawnTimer = 0.f;
        }
        weapon->spawnTimer += delta;
    }

    // Bullet behaviour
    if (COMPARE(e->type, TAG_BULLET)) {
        if (RunBulletPattern(e, delta))
            DestroyEntity(e);
    }

    // apply movement
    e->bounds.x += vel->x * delta;
    e->bounds.y += vel->y * delta;

    // apply drag
    // float offsetX = e->physics.drag * delta * SIGN(float, vel->x);
    // vel->x -= MIN(offsetX, vel->x);
    // float offsetY = e->physics.drag * delta * SIGN(float, vel->y);
    // vel->y -= MIN(offsetY, vel->y);
}

static usize LastCount = 0;
usize GetEntityCount()
{
    return LastCount;
}

usize UpdateAndRenderScene(Scene* scene, Texture canvas, float delta)
{
    usize count = 0;
    for (usize i = 0; i < MAX_ENTITY_PAGES; i++) {
        if (scene->entities[i] != NULL) {
            for (usize j = 0; j < ENTITIES_PER_PAGE; j++) {
                assert(scene->entities[i]);
                Entity* e = &scene->entities[i][j];
                assert(e);
                if (e->isActive) {
                    UpdateAndRenderEntity(scene, canvas, e, delta);
                    count++;
                }
            }
        } else {
            break;
        }
    }
    LastCount = count;
    return count;
}
