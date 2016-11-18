
#pragma once

#include "Math/Quad.h"
#include <string>

namespace editor
{
    struct Notification
    {
        Notification()
        { }

        Notification(const math::Quad& icon, const std::string& text, int time_left)
            : icon(icon),
              text(text),
              time_left(time_left)
        { }

        math::Quad icon;
        std::string text;
        int time_left;
    };
}
