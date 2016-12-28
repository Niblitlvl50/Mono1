
#pragma once

#include "MonoFwd.h"

namespace game
{
    struct SpawnEntityEvent
    {
        SpawnEntityEvent(const mono::IEntityPtr& entity)
            : entity(entity)
        { }

        const mono::IEntityPtr entity;
    };
}

