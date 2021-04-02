
#include "Bezier.h"

mono::Curve mono::MakeCurve(PathType type)
{
    const int n_start = (type == PathType::BEZIER_QUADRATIC) ? 3 : 4;
    const int n_points = 1;
    const int initial_points = n_start + (n_points - 1) * 2;

    return { type, std::vector<math::Vector>(initial_points) };
}

void mono::AddPoint(Curve& curve, const math::Vector& new_point)
{
    InsertPoint(curve, curve.points.size(), new_point);
}

void mono::InsertPoint(Curve& curve, int point_index, const math::Vector& new_point)
{
    curve.points.insert(curve.points.begin() + point_index, { new_point - math::Vector(-1, -1), new_point });
}

void mono::RemovePoint(Curve& curve, int point_index)
{
    int erase_start = point_index -1;
    int erase_end = point_index;

    const bool is_control_point = IsControlPoint(curve, point_index);
    if(is_control_point)
    {
        erase_start = point_index;
        erase_end = point_index +1;
    }

    curve.points.erase(curve.points.begin() + erase_start, curve.points.begin() + erase_end);
}

bool mono::IsControlPoint(const Curve& curve, int point_index)
{
    if(point_index == 0)
        return false;
    else if(point_index == 1)
        return true;
    else
        return ((point_index -2) % 2) != 0;
}
