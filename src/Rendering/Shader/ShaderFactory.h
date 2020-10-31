
#pragma once

#include "IShaderFactory.h"

namespace mono
{
    class ShaderFactory : public IShaderFactory
    {
    public:

        std::unique_ptr<IShader> CreateTextureShader() const override;
        std::unique_ptr<IShader> CreateColorShader() const override;
        std::unique_ptr<IShader> CreatePointSpriteShader() const override;
        std::unique_ptr<IShader> CreateScreenShader() const override;
        std::unique_ptr<IShader> CreateImGuiShader() const override;
        std::unique_ptr<IShader> CreateSpriteShader() const override;
    };
}

