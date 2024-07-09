
#pragma once

#include "Math/MathFwd.h"
#include "Rendering/RenderFwd.h"

#include <memory>

namespace mono
{
    struct TextureDrawBuffers
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> uv;
        std::unique_ptr<IElementBuffer> indices;
    };

    enum class AnchorPoint
    {
        CENTER,
        TOP_LEFT,
        BOTTOM_LEFT,
        TOP_RIGHT,
        BOTTOM_RIGHT
    };

    struct TextureBufferOptions
    {
        AnchorPoint anchor = AnchorPoint::CENTER;
        float pixels_per_meter = 1.0f;
    };

    TextureDrawBuffers BuildTextureDrawBuffers(const mono::ITexture* texture, const TextureBufferOptions& options);
    math::Vector CalculateOffsetFromCenter(mono::AnchorPoint anchor_point, const math::Vector& texture_size, float pixels_per_meter);
}
