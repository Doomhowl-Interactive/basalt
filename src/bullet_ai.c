#include "basalt.h"
#include "basalt_extra.h"
#include "bullet_common.h"

#define AI

// TODO Implement player movement using the AI system

usize GetEntityAIActionCount(const EntityAI* behaviour)
{
    for (uint index = 0; index < MAX_ACTIONS; index++) {
        if (behaviour->actions[index].function == NULL)
            return index;
    }
    return MAX_ACTIONS;
}

BULLET bool RunEntityAI(Entity* e, float delta)
{
    EntityAI* ai = &e->ai;
    FillInActionData(&ai->data, delta);

    if (ai->name == NULL) {
        return false;  // Entity has no AI, don't do anything.
    }

    if (ai->count == 0) {
        ai->count = GetEntityAIActionCount(ai);
    }

    if (ai->index >= ai->count) {
        return true;  // Finished all steps of the AI
    }

    EntityAIAction action = ai->actions[ai->index];
    EntityAIActionFunc actionFunc = action.function;
    assert(actionFunc);
    if (actionFunc(e, &ai->data, action.parameters)) {
        ai->index++;
        ResetActionData(&ai->data);
    }
    return false;
}

AI bool FlyTowards(Entity* e, ActionData* data, const int* args)
{
    const int BORDER_OFFSET = 150;
    const float DISTANCE_CHECK = 10.f;

    bool* initialized = &data->bools[0];
    if (!(*initialized)) {
        int flyInX = args[2];
        int flyInY = args[3];

        if (flyInX == 0 && flyInY == 0) {
            bool fromRightSide = (GetRandomNumber() % 100) > 50;
            flyInX = -BORDER_OFFSET;
            if (fromRightSide) {
                flyInX = WIDTH + BORDER_OFFSET;
            }
            flyInY = -BORDER_OFFSET;
        }

        SetEntityCenter(e, (float)flyInX, (float)flyInY);
        *initialized = true;
    }

    assert(data->difficulty > 0);
    float power = 100 * data->difficulty;
    Vec2 src = GetEntityCenter(e);
    Vec2 dest = {
        (float)args[0],
        (float)args[1],
    };

    float distanceLeft = Vec2DistanceSquared(src, dest);
    if (distanceLeft < DISTANCE_CHECK) {
        // TODO: Add some easing instead of instantly coming to a halt
        e->vel.x = 0;
        e->vel.y = 0;
        return true;
    }

    // adjust velocity to reach the target
    Vec2 direction = Vec2Towards(src, dest);
    e->vel.x = direction.x * power;
    e->vel.y = direction.y * power;

    return false;
}

AI bool SidewaysEight(const Entity* e, const ActionData* data, const int* args)
{
    // TODO:
    return false;
}

AI bool BehavePlayer(Entity* e, ActionData* data, const int* args)
{
    int* patternIndex = &data->ints[0];

    float moveSpeed = e->moveSpeed;
    e->vel.x = 0;
    e->vel.y = 0;

    // TODO: Use arrow keys instead to rip Touhou keyboard layout
    if (IsKeyDown(SDLK_LEFT)) {
        e->vel.x -= moveSpeed;
    }
    if (IsKeyDown(SDLK_RIGHT)) {
        e->vel.x += moveSpeed;
    }
    if (IsKeyDown(SDLK_UP)) {
        e->vel.y -= moveSpeed;
    }
    if (IsKeyDown(SDLK_DOWN)) {
        e->vel.y += moveSpeed;
    }

    // shoot if pressing Z
    for (usize i = 0; i < MAX_SPAWNERS; i++) {
         e->bulletSpawners[i].disabled = !IsKeyDown(SDLK_z);
    }

    // switch between bullet types for testing
    if (IsKeyPressed(SDLK_e)) {
        (*patternIndex)++;
        if (*patternIndex >= GetBulletPatternCount())
            *patternIndex = 0;

        for (int i = 0; i < MAX_SPAWNERS; i++) {
            BulletSpawner* spawner = &e->bulletSpawners[i];
            if (spawner == NULL)
                continue;
            spawner->patternToSpawn = GetBulletPattern(*patternIndex);
        }
    }

    return false;
}

// clang-format doesn't expand this shit
const EntityAI EntityAIBehaviours[] = { { "PlayerMovement", { { FlyTowards, { 400, 530, 400, 670 } }, { BehavePlayer } } },
                                        { "SidewaysEight", { { FlyTowards, { 150, 80 } }, { SidewaysEight, { 0 } } } },
                                        NULL };

BULLET const EntityAI* GetEntityAI(usize index)
{
    usize count = GetEntityAICount();
    if (index < count) {
        return &EntityAIBehaviours[index];
    }

    WARN("Did not find bullet pattern indexed %lu", index);
    return &EntityAIBehaviours[0];
}

BULLET const EntityAI* GetEntityAIByName(const char* name)
{
    for (const EntityAI* beh = EntityAIBehaviours; beh->name != NULL; beh++) {
        if (TextIsEqual(beh->name, name)) {
            return beh;
        }
    }
    WARN("Did not find bullet pattern with name %s", name);
    return GetEntityAI(0);
}

BULLET usize GetEntityAICount()
{
    static usize counter = 0;
    if (counter == 0) {
        for (const EntityAI* beh = EntityAIBehaviours; beh->name != NULL; beh++)
            counter++;
    }
    return counter;
}
