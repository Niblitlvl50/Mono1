
#pragma once

#include "IShaderFactory.h"

namespace mono
{
    class ShaderFactory : public IShaderFactory
    {
    public:

        std::unique_ptr<ITextureShader> CreateTextureShader() const override;
        std::unique_ptr<IColorShader> CreateColorShader() const override;
        std::unique_ptr<IPointSpriteShader> CreatePointSpriteShader() const override;
        std::unique_ptr<IImGuiShader> CreateImGuiShader() const override;
    };
}

