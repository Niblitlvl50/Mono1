
#pragma once

#include "Math/Vector.h"

#include <vector>
#include <cstdint>

namespace mono
{
    struct TextDefinition
    {
        std::vector<math::Vector> vertices;
        std::vector<math::Vector> texcoords;
        std::vector<uint16_t> indices;
    };
}
