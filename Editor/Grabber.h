
#pragma once

#include "Math/Vector2f.h"
#include <functional>

namespace editor
{
    struct Grabber
    {
        math::Vector2f position;
        bool hoover = false;
        std::function<void (const math::Vector2f&)> callback;
    };
}
