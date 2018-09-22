
#pragma once

#include "ShaderFwd.h"
#include <memory>

namespace mono
{
    class IShaderFactory
    {
    public:

        virtual ~IShaderFactory()
        { }

        virtual std::unique_ptr<ITextureShader> CreateTextureShader() const = 0;
        virtual std::unique_ptr<IColorShader> CreateColorShader() const = 0;
        virtual std::unique_ptr<IPointSpriteShader> CreatePointSpriteShader() const = 0;
    };
}
