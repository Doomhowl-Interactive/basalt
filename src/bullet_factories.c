#include "basalt_extra.h"
#include "bullet_common.h"
#include <math.h>

BULLET void InitPlayer(Entity* e, Vec2 pos)
{
    DEBUG("Spawned player at %f %f", pos.x, pos.y);
    e->sprite.texture = PlayerTexture;
    e->type = TAG_PLAYER;
    SetEntityCenter(e, pos.x - 48 / 2, pos.y);
    e->sprite.tint = 0x00FF00FF;
    e->ship.moveSpeed = 200;
    SetEntitySize(e, PlayerTexture.width, PlayerTexture.height);
    assert(PlayerTexture.width > 0 || PlayerTexture.height > 0);

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

    // DEBUG("Spawned bullet at %f %f", pos.x, pos.y);

    // set sprite
    e->type = TAG_BULLET;
    SetEntityCenter(e, pos.x, pos.y);
    SetEntitySize(e, pattern->texture->width, pattern->texture->height);
    e->sprite.sourceOffset.x = 0;
    e->sprite.sourceOffset.y = 0;
    e->sprite.texture = *pattern->texture;

    // copy bullet pattern
    e->bullet.pattern = *pattern;
    e->bullet.data.origin = pos;
    e->bullet.data.normal = normal;
    e->sprite.tint = 0xFFFFFFFF;
}
