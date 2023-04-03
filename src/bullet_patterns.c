#include "basalt_extra.h"
#include "bullet_common.h"

#define DIFFICULTY 2

#define PATTERN
#define END -1000

uint GetBulletPatternActionCount(BulletPattern* pattern)
{
    uint index;
    for (index = 0; index < MAX_ENTITIES; index++)
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
    if ((*action.function)(e, e->bullet.data, DIFFICULTY, action.parameters))
    {
        // on bullet action done
        e->bullet.curPatternIndex++;
    }
    return false;
}

//typedef bool (*BulletActionFunc)(Entity* entity, BulletData memory, int difficulty, const int* args);

PATTERN bool MoveBulletStraight(Entity* e, BulletData memory, int difficulty, const int* args)
{
    int angleDeg = args[0];
    int duration = args[1];

    float power = 150 + difficulty * 30;

    e->physics.vel.x = cos(DEG2RAD(angleDeg))*power;
    e->physics.vel.y = sin(DEG2RAD(angleDeg))*power;

    return memory.timer > duration;
}

PATTERN bool MoveBulletHelix(Entity* e, BulletData memory, int difficulty, const int* args)
{
    // TODO:
    return false;
}

const BulletPattern PlayerBullet = {
    {
        {
            MoveBulletStraight,
            { -90, 5, END }
        }
    }
};

const BulletPattern HelixBullet = {
    {
        {
            MoveBulletHelix,
            { 40, 40, END }
        }
    }
};
