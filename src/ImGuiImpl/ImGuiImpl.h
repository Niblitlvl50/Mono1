
#pragma once

#include "Math/Quad.h"
#include "imgui/imgui.h"

struct ImageCoords
{
    ImVec2 uv1;
    ImVec2 uv2;
};

inline ImageCoords QuadToImageCoords(const math::Quad& input)
{
    ImageCoords coords;
    coords.uv1 = ImVec2(input.mA.x, input.mB.y);
    coords.uv2 = ImVec2(input.mB.x, input.mA.y);

    return coords;
}
