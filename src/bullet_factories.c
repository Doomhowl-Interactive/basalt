#include <math.h>

#include "basalt_extra.h"
#include "bullet_common.h"

BULLET void InitPlayer(Entity* e, Vec2 pos)
{
    DEBUG("Spawned player at %f %f", pos.x, pos.y);
    e->texture = PlayerTexture;
    e->type = TAG_PLAYER;
    SetEntityCenter(e, pos.x - 48 / 2, pos.y);
    e->tint = WHITE;
    e->moveSpeed = 200;
    SetEntitySize(e, PlayerTexture.width, PlayerTexture.height);
    assert(PlayerTexture.width > 0 || PlayerTexture.height > 0);

    // Bullet spawners
    double outwardsAngleDeg = 40;
    double distanceFromPlayer = 45;
    uint spawnerCount = 5;

    double anglePerSpawner = outwardsAngleDeg / spawnerCount;
    for (uint i = 0; i < spawnerCount; i++) {
        double angle = -90 - outwardsAngleDeg * 0.5f + anglePerSpawner * i + anglePerSpawner * 0.5f;

        BulletSpawner* spawner = &e->bulletSpawners[i];
        spawner->interval = 0.1f;
        spawner->normal.x = cos(DEG2RAD(angle));
        spawner->normal.y = sin(DEG2RAD(angle));
        spawner->offset = Vec2Scale(spawner->normal, distanceFromPlayer);
        spawner->patternToSpawn = GetBulletPatternByName("PlayerBullet1");
    }
}

BULLET void InitBullet(Entity* e, const BulletPattern* pattern, Vec2 pos, Vec2 normal)
{
    assert(pattern);

    // DEBUG("Spawned bullet at %f %f", pos.x, pos.y);

    // set sprite
    e->type = TAG_BULLET;
    SetEntityCenter(e, pos.x, pos.y);
    SetEntitySize(e, pattern->texture->width, pattern->texture->height);
    e->sourceOffset.x = 0;
    e->sourceOffset.y = 0;
    e->texture = *pattern->texture;

    // copy bullet pattern
    e->bulletPattern = *pattern;
    e->bulletData.origin = pos;
    e->bulletData.normal = normal;
    e->tint = 0xFFFFFFFF;
}
