
#pragma once

#include "Math/MathFwd.h"
#include "Rendering/RenderFwd.h"
#include <memory>

namespace mono
{
    struct SpriteDrawBuffers
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> offsets;
        std::unique_ptr<IRenderBuffer> uv;
        std::unique_ptr<IRenderBuffer> uv_flipped;
        std::unique_ptr<IRenderBuffer> heights;
        int vertices_per_sprite;
    };

    struct SpriteShadowBuffers
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> uv;
    };

    SpriteDrawBuffers BuildSpriteDrawBuffers(const mono::SpriteData* sprite_data, const char* debug_context);
    SpriteShadowBuffers BuildSpriteShadowBuffers(const math::Vector& shadow_offset, float shadow_radius);
}
