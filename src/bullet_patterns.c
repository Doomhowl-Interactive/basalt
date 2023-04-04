#include "basalt_extra.h"
#include "bullet_common.h"

#define DIFFICULTY 2

#define PATTERN

uint GetBulletPatternActionCount(BulletPattern* pattern)
{
    for (uint index = 0; index < MAX_ENTITIES; index++)
    {
        if (pattern->actions[index].function == NULL)
            return index;
    }
    return MAX_ENTITIES;
}

BULLET bool RunBulletPattern(Entity* e, float delta)
{
    BulletPattern* pattern = &e->bullet.pattern;

    if (pattern->count == 0)
        pattern->count = GetBulletPatternActionCount(pattern);

    if (e->bullet.curPatternIndex >= pattern->count)
        return true;

    BulletAction action = pattern->actions[e->bullet.curPatternIndex];
    assert(action.function);
    e->bullet.data.timer += delta;
    e->bullet.data.delta = delta;
    e->sprite.tint = action.tint;
    if ((*action.function)(e, &e->bullet.data, DIFFICULTY, action.parameters))
    {
        // on bullet action done
        e->bullet.curPatternIndex++;
    }
    return false;
}

//typedef bool (*BulletActionFunc)(Entity* entity, BulletData memory, int difficulty, const int* args);

// TODO: Add end condition instead of passing duration integer everywhere
PATTERN bool MoveBulletStraight(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int duration = args[0];

    float power = 150 + difficulty * 30;

    e->physics.vel.x = data->normal.x*power;
    e->physics.vel.y = data->normal.y*power;

    return data->timer > duration;
}

PATTERN bool MoveBulletOceanWave(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int segHeight = args[1];

    float power = 150 + difficulty * 30;

    // TODO: ResetVelocity()
    e->physics.vel.x = 0;
    e->physics.vel.y = 0;

    float distance = data->timer * power;

    e->sprite.pos.x = (data->origin.x + data->normal.x * distance) + cos(data->timer*10) * segWidth;
    e->sprite.pos.y = (data->origin.y + data->normal.y * distance) + sin(data->timer*10) * segHeight;

    return false;
}

PATTERN bool MoveBulletSowing(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int speed = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);

    float distance = data->timer * power;

    e->sprite.pos.x = (data->origin.x + data->normal.x * distance) + cos(data->timer*speed) * segWidth;
    e->sprite.pos.y = (data->origin.y + data->normal.y * distance);

    return false;
}

PATTERN bool MoveBulletStaircase(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int speed = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);

    float distance = data->timer * power;
    e->sprite.pos.x = (data->origin.x + data->normal.x * distance);
    e->sprite.pos.y = (data->origin.y + data->normal.y * distance) + cos(data->timer*speed) * segWidth;

    return false;
}

PATTERN bool MoveBulletSnake(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int yFlip = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);

    return false;
}

const BulletPattern PlayerBullet = {
    {
        {
            MoveBulletStraight,
            0xFFBB00FF,
            { 5 },
        }
    }
};

const BulletPattern PlayerBullet2 = {
    {
        {
            MoveBulletOceanWave,
            0xAAAAFFFF,
            { 40, 40 }
        }
    }
};

const BulletPattern PlayerBullet3 = {
    {
        {
            MoveBulletStaircase,
            0x0022DDFF,
            { 10, -1 }
        }
    }
};

const BulletPattern PlayerBullet4 = {
    {
        {
            MoveBulletSnake,
            0x22FF22FF,
            { 10, -1 }
        }
    }
};
