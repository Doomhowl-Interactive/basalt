#include "basalt_extra.h"
#include "bullet_common.h"

#define PATTERN
#define END -1000

BULLET bool RunBulletPattern(Entity* target)
{
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

    // TODO:
    return false;
}

PATTERN bool MoveBulletHelix(Entity* e, BulletData memory, int difficulty, const int* args)
{
    // TODO:
    return false;
}

const BulletPatterns PlayerBullet = {
    0, 0,
    {
        {
            MoveBulletStraight,
            { 0, 5, END }
        }
    }
};

const BulletPatterns HelixBullet = {
    0, 0,
    {
        {
            MoveBulletHelix,
            { 40, 40, END }
        }
    }
};
