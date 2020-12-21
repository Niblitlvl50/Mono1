
#pragma once

#include "Rendering/RenderFwd.h"
#include <memory>
#include <cstdint>

namespace mono
{
    class ScreenShader
    {
    public:

        static std::unique_ptr<IShader> MakeShader();
        static void FadeCorners(IShader* shader, bool enable);
        static uint32_t GetPositionAttribute(IShader* shader);
        static uint32_t GetTextureAttribute(IShader* shader);
    };
}
