
#pragma once

#include "Math/Vector.h"
#include <vector>

namespace mono
{
    enum class CurveType
    {
        QUADRATIC,
        CUBIC
    };

    struct Curve
    {
        CurveType type;
        std::vector<math::Vector> points;
    };

    Curve MakeCurve(CurveType type);

    void AddPoint(Curve& curve, const math::Vector& new_point);
    void InsertPoint(Curve& curve, int point_index, const math::Vector& new_point);
    void RemovePoint(Curve& curve, int point_index);

    bool IsControlPoint(const Curve& curve, int point_index);
}
