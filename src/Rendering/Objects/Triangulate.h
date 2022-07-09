
#pragma once

#include "Math/Vector.h"
#include <vector>

namespace mono
{
    struct TriangulatedPoints
    {
        std::vector<math::Vector> vertices;
        std::vector<uint16_t> triangles;
    };

    TriangulatedPoints Triangulate(const std::vector<math::Vector>& points);
}
