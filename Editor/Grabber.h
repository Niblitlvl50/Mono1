
#pragma once

#include "Math/Vector.h"
#include <functional>

namespace editor
{
    struct Grabber
    {
        math::Vector position;
        bool hoover = false;
        std::function<void (const math::Vector&)> callback;
    };
}
