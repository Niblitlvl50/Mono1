
#pragma once

#include <memory>

namespace mono
{
    class IShader;

    class IShaderFactory
    {
    public:

        virtual ~IShaderFactory() = default;
        virtual std::unique_ptr<IShader> CreateTextureShader() const = 0;
        virtual std::unique_ptr<IShader> CreateColorShader() const = 0;
        virtual std::unique_ptr<IShader> CreatePointSpriteShader() const = 0;
        virtual std::unique_ptr<IShader> CreateImGuiShader() const = 0;
    };
}
