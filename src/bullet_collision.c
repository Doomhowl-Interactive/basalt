#include "basalt.h"
#include "bullet_common.h"

func void OnTouchedEntity(Entity* e, Entity* other)
{
    INFO("%d touched %d", e->id, other->id);
}

static Entity* entity;
func void CheckCollision(Entity* it, int i)
{
    if (entity->id == it->id) {
        return;
    }

    if (EntityHasFlag(it, entity->ignoreFlags)) {
        return;
    }

    if (RectFOverlaps(entity->bounds, it->bounds)) {
        OnTouchedEntity(it, entity);
    }
}

BULLET void CheckCollisionOfEntity(Entity* e, Scene* scene)
{
    if (!e->isToucher) {
        return;
    }

    entity = e;
    ForeachSceneEntity(scene, CheckCollision);
}
