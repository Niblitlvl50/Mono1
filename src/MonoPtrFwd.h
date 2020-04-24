
#pragma once

#include "MonoFwd.h"
#include <memory>

namespace mono
{
    using ISoundPtr = std::unique_ptr<ISound>;
    using IPathPtr = std::unique_ptr<IPath>;
}
