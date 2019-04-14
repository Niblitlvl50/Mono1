
#pragma once

#include <vector>
#include "Rendering/Color.h"

namespace mono
{
    struct TextDefinition
    {
        std::vector<float> vertices;
        std::vector<float> texcoords;
        mono::Color::RGBA color;
        uint32_t chars;
    };
}
