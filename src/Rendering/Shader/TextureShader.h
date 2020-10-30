
#pragma once

#include "Rendering/RenderFwd.h"
#include <memory>
#include <cstdint>

namespace mono
{
    class TextureShader
    {
    public:

        static std::unique_ptr<IShader> MakeShader();
        static void SetShade(IShader* shader, const mono::Color::RGBA& color);
        static void SetAlphaTexture(IShader* shader, bool is_alpha);
        static void SetWindSway(IShader* shader, bool enable_wind);
        static void SetTextureOffset(IShader* shader, float offset);
        static uint32_t GetPositionAttribute(IShader* shader);
        static uint32_t GetTextureAttribute(IShader* shader);
    };
}
