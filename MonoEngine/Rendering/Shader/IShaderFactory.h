
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

        virtual std::shared_ptr<ITextureShader> CreateTextureShader() const = 0;
        virtual std::shared_ptr<IColorShader> CreateColorShader() const = 0;
        virtual std::shared_ptr<IMorphingShader> CreateMorphingShader() const = 0;
        virtual std::shared_ptr<IPointSpriteShader> CreatePointSpriteShader() const = 0;
    };
}
