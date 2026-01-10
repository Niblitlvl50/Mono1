
#pragma once

#include "Rendering/RenderFwd.h"
#include "Math/MathFwd.h"
#include <memory>
#include <vector>

namespace mono
{
    struct PrimitiveDrawBuffers
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> colors;
        std::unique_ptr<IElementBuffer> indices;
    };

    PrimitiveDrawBuffers BuildCircleDrawBuffers(const math::Vector& size, int segments, const mono::Color::RGBA& color);

    PrimitiveDrawBuffers BuildPointsDrawBuffers(const std::vector<math::Vector>& points, const std::vector<mono::Color::RGBA>& colors);
    PrimitiveDrawBuffers BuildMutablePointsDrawBuffers(const std::vector<math::Vector>& points, const std::vector<mono::Color::RGBA>& colors);

    void UpdateMutablePoints(const std::vector<math::Vector>& points, PrimitiveDrawBuffers& draw_buffers);
    void UpdateMutablePointColors(const std::vector<mono::Color::RGBA>& colors, mono::PrimitiveDrawBuffers& draw_buffers);
}
