
#pragma once

#include "IShaderFactory.h"

namespace mono
{
    class ShaderFactory : public IShaderFactory
    {
    public:

        virtual std::unique_ptr<ITextureShader> CreateTextureShader() const;
        virtual std::unique_ptr<IColorShader> CreateColorShader() const;
        virtual std::unique_ptr<IPointSpriteShader> CreatePointSpriteShader() const;
    };
}

