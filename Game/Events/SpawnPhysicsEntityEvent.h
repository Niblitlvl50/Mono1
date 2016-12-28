
#pragma once

#include "MonoFwd.h"
#include "MonoPtrFwd.h"

namespace game
{
    struct SpawnPhysicsEntityEvent
    {
        SpawnPhysicsEntityEvent(const mono::IPhysicsEntityPtr& entity, int layer)
            : entity(entity),
              layer(layer)
        { }

        mono::IPhysicsEntityPtr entity;
        const int layer;
    };
}
