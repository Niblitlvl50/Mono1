
#pragma once

#include "MonoFwd.h"
#include <memory>

namespace mono
{
    using ICameraPtr = std::shared_ptr<ICamera>;
    using IEntityPtr = std::shared_ptr<IEntity>;
    using IUpdatablePtr = std::shared_ptr<IUpdatable>;
    using IZonePtr = std::unique_ptr<IZone>;
    using ISoundPtr = std::unique_ptr<ISound>;
    using IPathPtr = std::unique_ptr<IPath>;
}
