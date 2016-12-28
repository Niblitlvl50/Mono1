
#pragma once

#include "Physics/CMFwd.h"

namespace game
{
    struct SpawnConstraintEvent
    {
        SpawnConstraintEvent(const mono::IConstraintPtr& constraint)
            : constraint(constraint)
        { }

        const mono::IConstraintPtr constraint;
    };

    struct DespawnConstraintEvent
    {
        DespawnConstraintEvent(const mono::IConstraintPtr& constraint)
            : constraint(constraint)
        { }

        const mono::IConstraintPtr constraint;
    };
}
