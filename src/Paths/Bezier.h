
#pragma once

#include "PathTypes.h"
#include "Math/Vector.h"
#include <vector>

namespace mono
{
    struct Curve
    {
        PathType type;
        std::vector<math::Vector> points;
    };

    Curve MakeCurve(PathType type);

    void AddPoint(Curve& curve, const math::Vector& new_point);
    void InsertPoint(Curve& curve, int point_index, const math::Vector& new_point);
    void RemovePoint(Curve& curve, int point_index);

    bool IsControlPoint(const Curve& curve, int point_index);
}
