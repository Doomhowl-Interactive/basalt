#include "basalt_extra.h"
#include "bullet_common.h"

#include <math.h>

#define DIFFICULTY 2

#define PATTERN
#define ENDING

ENDING bool EndBulletOOB(Entity* e, BulletData* data, int difficulty, const int* args)
{
    Vec2 pos = GetEntityCenter(e);;
    const int OOB = 100;
    return (pos.x < -OOB || pos.y < -OOB || pos.x > WIDTH+OOB || pos.y > HEIGHT+OOB);
}

// TODO: Add end condition instead of passing duration integer everywhere
PATTERN void MoveBulletStraight(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int duration = args[0];

    float power = 150 + difficulty * 30;

    e->vel.x = data->normal.x*power;
    e->vel.y = data->normal.y*power;
}

PATTERN void MoveBulletOceanWave(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int segHeight = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);
    float distance = data->timer * power;

    float centerX = (data->origin.x + data->normal.x * distance) + cos(data->timer*10) * segWidth;
    float centerY = (data->origin.y + data->normal.y * distance) + sin(data->timer*10) * segHeight;
    SetEntityCenter(e, centerX, centerY);
}

PATTERN void MoveBulletSowing(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int speed = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);

    float distance = data->timer * power;

    float centerX = (data->origin.x + data->normal.x * distance) + cos(data->timer*speed) * segWidth;
    float centerY = (data->origin.y + data->normal.y * distance);
    SetEntityCenter(e, centerX, centerY);
}

PATTERN void MoveBulletStaircase(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int speed = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);

    float distance = data->timer * power;
    Vec2 center = GetEntityCenter(e);
    float centerX = center.x = (data->origin.x + data->normal.x * distance);
    float centerY = center.y = (data->origin.y + data->normal.y * distance) + cos(data->timer*speed) * segWidth;
    SetEntityCenter(e, centerX, centerY);
}

PATTERN void MoveBulletSnake(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth  = args[0];
    int yFlip = args[1];

    float power = 150 + difficulty * 30;

    e->vel.x = cos(data->timer*segWidth)*power;
    e->vel.y = yFlip * ABS(float, sin(data->timer*segWidth)*power);
}

// core system
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
    BulletPattern* pattern = &e->bulletPattern;

    if (pattern->count == 0)
        pattern->count = GetBulletPatternActionCount(pattern);

    if (pattern->index >= pattern->count)
        return true;

    BulletAction action = pattern->actions[pattern->index];
    assert(action.function);
    e->bulletData.timer += delta;
    e->bulletData.delta = delta;
    e->tint = action.tint;

    // process bullet action
    BulletActionFunc actionFunc = action.function;
    (*actionFunc)(e, &e->bulletData, DIFFICULTY, action.parameters);

    BulletActionEndFunc endFunc = action.endFunction;
    if ((*endFunc)(e, &e->bulletData, DIFFICULTY, action.parameters))
    {
        // on bullet action done
        pattern->index++;
    }
    return false;
}

const BulletPattern BulletPatterns[] = {
    {
        "PlayerBullet1",
        {
            {
                MoveBulletStraight,
                EndBulletOOB,
                0xFFBB00FF,
                { 5 },
            }
        },
        &BulletPlacholderTexture,
    },
    {
        "PlayerBullet2",
        {
            {
                MoveBulletOceanWave,
                EndBulletOOB,
                0xAAAAFFFF,
                { 40, 40 }
            }
        },
        &BulletPlacholderTexture,
    },
    {
        "PlayerBullet3",
        {
            {
                MoveBulletStaircase,
                EndBulletOOB,
                0x0022DDFF,
                { 10, -1 }
            }
        },
        &BulletPlacholderTexture
    },
    {
        "PlayerBullet4",
        {
            {
                MoveBulletSnake,
                EndBulletOOB,
                0x22FF22FF,
                { 10, -1 }
            }
        },
        &BulletPlacholderTexture
    },
    { NULL }
};

BULLET const BulletPattern* GetBulletPattern(usize index)
{
    usize count = GetBulletPatternCount();
    if (index < count)
        return &BulletPatterns[index];

    WARN("Did not find bullet pattern indexed %d", index);
    return &BulletPatterns[0];
}

BULLET const BulletPattern* GetBulletPatternByName(const char* name)
{
    for (const BulletPattern* pat = BulletPatterns; pat->name != NULL; pat++)
    {
        if (strcmp(pat->name,name) == 0)
            return pat;
    }
    WARN("Did not find bullet pattern with name %s", name);
    return GetBulletPattern(0);
}

BULLET usize GetBulletPatternCount()
{
    static usize counter = 0;
    if (counter == 0)
    {
        for (const BulletPattern* pat = BulletPatterns; pat->name != NULL; pat++)
            counter++;
    }
    return counter;
}
