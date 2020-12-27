
#pragma once

#include "Math/Vector.h"
#include "imgui/imgui.h"

struct ImageCoords
{
    ImVec2 uv1;
    ImVec2 uv2;
};

inline ImageCoords QuadToImageCoords(const math::Vector& uv_upper_left, const math::Vector& uv_lower_righ)
{
    ImageCoords coords;
    coords.uv1 = ImVec2(uv_upper_left.x, uv_lower_righ.y);
    coords.uv2 = ImVec2(uv_lower_righ.x, uv_upper_left.y);

    return coords;
}
