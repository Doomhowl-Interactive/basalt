#include "basalt_extra.h"
#include "bullet_common.h"
#include <math.h>

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
