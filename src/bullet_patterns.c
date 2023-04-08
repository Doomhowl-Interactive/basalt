#include "basalt_extra.h"
#include "bullet_common.h"

#define DIFFICULTY 2

#define PATTERN
#define ENDING

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

    // process bullet action
    BulletActionFunc actionFunc = action.function;
    (*actionFunc)(e, &e->bullet.data, DIFFICULTY, action.parameters);

    BulletActionEndFunc endFunc = action.endFunction;
    if ((*endFunc)(e, &e->bullet.data, DIFFICULTY, action.parameters))
    {
        // on bullet action done
        e->bullet.curPatternIndex++;
    }
    return false;
}

ENDING bool EndBulletOOB(Entity* e, BulletData* data, int difficulty, const int* args)
{
    const int OOB = 100;
    int x = e->sprite.pos.x;
    int y = e->sprite.pos.y;
    return (x < -OOB || y < -OOB || x > WIDTH+OOB || y > HEIGHT+OOB);
}

// TODO: Add end condition instead of passing duration integer everywhere
PATTERN void MoveBulletStraight(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int duration = args[0];

    float power = 150 + difficulty * 30;

    e->physics.vel.x = data->normal.x*power;
    e->physics.vel.y = data->normal.y*power;
}

PATTERN void MoveBulletOceanWave(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int segHeight = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);
    float distance = data->timer * power;

    e->sprite.pos.x = (data->origin.x + data->normal.x * distance) + cos(data->timer*10) * segWidth;
    e->sprite.pos.y = (data->origin.y + data->normal.y * distance) + sin(data->timer*10) * segHeight;
}

PATTERN void MoveBulletSowing(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int speed = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);

    float distance = data->timer * power;

    e->sprite.pos.x = (data->origin.x + data->normal.x * distance) + cos(data->timer*speed) * segWidth;
    e->sprite.pos.y = (data->origin.y + data->normal.y * distance);
}

PATTERN void MoveBulletStaircase(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int speed = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);

    float distance = data->timer * power;
    e->sprite.pos.x = (data->origin.x + data->normal.x * distance);
    e->sprite.pos.y = (data->origin.y + data->normal.y * distance) + cos(data->timer*speed) * segWidth;
}

PATTERN void MoveBulletSnake(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int yFlip = args[1];

    float power = 150 + difficulty * 30;

    e->physics.vel.x = cos(data->timer*segWidth)*power;
    e->physics.vel.y = yFlip * ABS(float, sin(data->timer*segWidth)*power);
}

const BulletPattern PlayerBullet = {
    {
        {
            MoveBulletStraight,
            EndBulletOOB,
            0xFFBB00FF,
            { 5 },
        }
    },
    &BulletPlacholderTexture,
};

const BulletPattern PlayerBullet2 = {
    {
        {
            MoveBulletOceanWave,
            EndBulletOOB,
            0xAAAAFFFF,
            { 40, 40 }
        }
    },
    &BulletPlacholderTexture,
};

const BulletPattern PlayerBullet3 = {
    {
        {
            MoveBulletStaircase,
            EndBulletOOB,
            0x0022DDFF,
            { 10, -1 }
        }
    },
    &BulletPlacholderTexture
};

const BulletPattern PlayerBullet4 = {
    {
        {
            MoveBulletSnake,
            EndBulletOOB,
            0x22FF22FF,
            { 10, -1 }
        }
    },
    &BulletPlacholderTexture
};
