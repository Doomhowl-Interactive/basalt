#include "basalt.h"
#include "basalt_extra.h"
#include "bullet_common.h"

// TODO Implement player movement using the AI system

usize GetEntityAIActionCount(const EntityAIBehaviour* behaviour)
{
    for (uint index = 0; index < MAX_ACTIONS; index++) {
        if (behaviour->actions[index].function == NULL)
            return index;
    }
    return MAX_ACTIONS;
}

BULLET bool RunEntityAI(Entity* e, float delta)
{
    EntityAIBehaviour* ai = &e->ai;
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
    }
    return false;
}
