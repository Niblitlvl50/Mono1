
#pragma once

#include "MonoFwd.h"
#include <memory>

namespace mono
{
    using IEntityPtr = std::shared_ptr<IEntity>;
    using IPhysicsEntityPtr = std::shared_ptr<IPhysicsEntity>;
    using IUpdatablePtr = std::shared_ptr<IUpdatable>;
    using IZonePtr = std::shared_ptr<IZone>;
    using IActionPtr = std::shared_ptr<IAction>;
    using ISoundPtr = std::shared_ptr<ISound>;
    using IPathPtr = std::shared_ptr<IPath>;
}
