
#pragma once

#include "Rendering/RenderFwd.h"
#include "Math/MathFwd.h"
#include <memory>

namespace mono
{
    struct PrimitiveDrawBuffers
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> colors;
        std::unique_ptr<IElementBuffer> indices;
    };

    PrimitiveDrawBuffers BuildCircleDrawBuffers(const math::Vector& size, int segments, const mono::Color::RGBA& color);
}
