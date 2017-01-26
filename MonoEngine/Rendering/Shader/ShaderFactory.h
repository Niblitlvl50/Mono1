
#pragma once

#include "IShaderFactory.h"

namespace mono
{
    class ShaderFactory : public IShaderFactory
    {
    public:

        virtual std::shared_ptr<ITextureShader> CreateTextureShader() const;
        virtual std::shared_ptr<IColorShader> CreateColorShader() const;
        virtual std::shared_ptr<IMorphingShader> CreateMorphingShader() const;
        virtual std::shared_ptr<IPointSpriteShader> CreatePointSpriteShader() const;
    };
}

