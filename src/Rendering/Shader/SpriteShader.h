
#pragma once

#include "Rendering/RenderFwd.h"
#include <memory>
#include <cstdint>

namespace mono
{
    class SpriteShader
    {
    public:

        static std::unique_ptr<IShader> MakeShader();
        static void SetShade(IShader* shader, const mono::Color::RGBA& color);
        static void SetWindSway(IShader* shader, bool enable_wind);
        static void SetFlashSprite(IShader* shader, bool flash_sprite);
        static uint32_t GetPositionAttribute(IShader* shader);
        static uint32_t GetTextureAttribute(IShader* shader);
        static uint32_t GetHeightAttribute(IShader* shader);
    };
}
