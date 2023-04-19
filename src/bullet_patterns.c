#include <math.h>

#include "basalt.h"
#include "basalt_extra.h"
#include "bullet_common.h"

#define DIFFICULTY 2

#define PATTERN
#define ENDING

ENDING bool EndBulletOOB(Entity* e, BulletData* data, int difficulty, const int* args)
{
    Vec2 pos = GetEntityCenter(e);
    const int OOB = 100;
    return (pos.x < -OOB || pos.y < -OOB || pos.x > WIDTH + OOB || pos.y > HEIGHT + OOB);
}

ENDING bool EndBulletTimer(Entity* e, BulletData* data, int difficulty, const int* args)
{
    return (data->timer > args[0]);
}

ENDING bool EndBulletInstantly(Entity* e, BulletData* data, int difficulty, const int* args)
{
    return true;
}

PATTERN void SplitBulletCircle(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int amount = args[0];
    float degsPerSeg = 360.f / amount;
    for (usize i = 0; i < amount; i++) {
        float rads = DEG2RAD(degsPerSeg * i);
        Vec2 normal = {
            cos(rads),
            sin(rads),
        };

        // Copy pattern from parent, but skip current action.
        Entity* ent = CreateEntity(e->scene);
        BulletPattern pattern = e->bulletPattern;
        pattern.index++;
        Vec2 spawnPos = GetEntityCenter(e);
        InitBullet(ent, &pattern, spawnPos, normal);
    }
}

PATTERN void MoveBulletStraight(Entity* e, BulletData* data, int difficulty, const int* args)
{
    float power = 150 + difficulty * 30;

    e->vel.x = data->normal.x * power;
    e->vel.y = data->normal.y * power;
}

PATTERN void MoveBulletOceanWave(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth = args[0];
    int segHeight = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);
    float distance = data->timer * power;

    float centerX = (data->origin.x + data->normal.x * distance) + cos(data->timer * 10) * segWidth;
    float centerY = (data->origin.y + data->normal.y * distance) + sin(data->timer * 10) * segHeight;
    SetEntityCenter(e, centerX, centerY);
}

PATTERN void MoveBulletSowing(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth = args[0];
    int speed = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);

    float distance = data->timer * power;

    float centerX = (data->origin.x + data->normal.x * distance) + cos(data->timer * speed) * segWidth;
    float centerY = (data->origin.y + data->normal.y * distance);
    SetEntityCenter(e, centerX, centerY);
}

PATTERN void MoveBulletStaircase(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth = args[0];
    int speed = args[1];

    float power = 150 + difficulty * 30;

    ResetEntityVelocity(e);

    float distance = data->timer * power;
    Vec2 center = GetEntityCenter(e);
    float centerX = center.x = (data->origin.x + data->normal.x * distance);
    float centerY = center.y = (data->origin.y + data->normal.y * distance) + cos(data->timer * speed) * segWidth;
    SetEntityCenter(e, centerX, centerY);
}

PATTERN void MoveBulletSnake(Entity* e, BulletData* data, int difficulty, const int* args)
{
    int segWidth = args[0];
    int yFlip = args[1];

    float power = 150 + difficulty * 30;

    e->vel.x = cos(data->timer * segWidth) * power;
    e->vel.y = yFlip * ABS(float, sin(data->timer * segWidth) * power);
}

// core system
uint GetBulletPatternActionCount(BulletPattern* pattern)
{
    for (uint index = 0; index < MAX_ENTITIES; index++) {
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
    if ((*endFunc)(e, &e->bulletData, DIFFICULTY, action.parameters)) {
        // on bullet action done
        pattern->index++;
    }
    return false;
}

const BulletPattern BulletPatterns[]
    = { { "Stresstest",
          {
              {
                  MoveBulletStraight,
                  EndBulletTimer,
                  0xFFAFFFFF,
                  { 1 },
              },
              {
                  SplitBulletCircle,
                  EndBulletInstantly,
                  0xFFAAAAFF,
                  { 300 },
              },
              {
                  MoveBulletStraight,
                  EndBulletOOB,
                  0xFFAFFFFF,
                  { 10 },
              },
          } },

        { "Firework",
          {
              {
                  MoveBulletStraight,
                  EndBulletTimer,
                  0xFFAAAAFF,
                  { 1 },
              },
              {
                  SplitBulletCircle,
                  EndBulletInstantly,
                  0xFFAAAAFF,
                  { 30 },
              },
              {
                  MoveBulletStraight,
                  EndBulletOOB,
                  0xFF0000FF,
                  { 10 },
              },
          } },
        { "Firework-Double",
          {
              {
                  MoveBulletStraight,
                  EndBulletTimer,
                  0xFFAAAAFF,
                  { 1 },
              },
              {
                  SplitBulletCircle,
                  EndBulletInstantly,
                  0xAAFFAAFF,
                  { 5 },
              },
              {
                  MoveBulletStraight,
                  EndBulletTimer,
                  0xFF0000FF,
                  { 1 },
              },
              {
                  SplitBulletCircle,
                  EndBulletInstantly,
                  0x00FFAAFF,
                  { 3 },
              },
              {
                  MoveBulletStraight,
                  EndBulletOOB,
                  0x0000FFFF,
                  { 10 },
              },
          } },
        {
            "PlayerBullet1",
            {
                {
                    MoveBulletStraight,
                    EndBulletOOB,
                    0xFFBB00FF,
                    { 5 },
                },
            },
            SPR_BULLET_PLACEHOLDER,
        },
        {
            "PlayerBullet2",
            { { MoveBulletOceanWave, EndBulletOOB, 0xAAAAFFFF, { 40, 40 } } },
            SPR_BULLET_PLACEHOLDER,
        },
        { "PlayerBullet3", { { MoveBulletStaircase, EndBulletOOB, 0x0022DDFF, { 10, -1 } } }, SPR_BULLET_PLACEHOLDER },
        { "PlayerBullet4", { { MoveBulletSnake, EndBulletOOB, 0x22FF22FF, { 10, -1 } } }, SPR_BULLET_PLACEHOLDER },
        { NULL } };

BULLET const BulletPattern* GetBulletPattern(usize index)
{
    usize count = GetBulletPatternCount();
    if (index < count)
        return &BulletPatterns[index];

    WARN("Did not find bullet pattern indexed %lu", index);
    return &BulletPatterns[0];
}

BULLET const BulletPattern* GetBulletPatternByName(const char* name)
{
    for (const BulletPattern* pat = BulletPatterns; pat->name != NULL; pat++) {
        if (strcmp(pat->name, name) == 0)
            return pat;
    }
    WARN("Did not find bullet pattern with name %s", name);
    return GetBulletPattern(0);
}

BULLET usize GetBulletPatternCount()
{
    static usize counter = 0;
    if (counter == 0) {
        for (const BulletPattern* pat = BulletPatterns; pat->name != NULL; pat++)
            counter++;
    }
    return counter;
}
