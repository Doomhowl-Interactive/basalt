#include "basalt.h"
#include "bullet_common.h"

func void OnTouchedEntity(Entity* e, Entity* sender)
{
    if (EntityHasFlag(e, FLAG_PLAYER)) {
        EntityDamage(e);
    }
}

static Entity* sender;
func void CheckCollision(Entity* listener, int i)
{
    if (sender->id == listener->id) {
        return;
    }

    if (EntityHasFlag(listener, sender->ignoreFlags)) {
        return;
    }

    if (RectFOverlaps(sender->bounds, listener->bounds)) {
        OnTouchedEntity(listener, sender);
    }
}

BULLET void CheckCollisionOfEntity(Entity* e, Scene* scene)
{
    if (!e->isToucher) {
        return;
    }

    sender = e;
    ForeachSceneEntity(scene, CheckCollision);
}
