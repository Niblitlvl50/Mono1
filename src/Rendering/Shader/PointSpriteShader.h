
#pragma once

#include <memory>
#include <cstdint>

namespace mono
{
    class IShader;

    class PointSpriteShader
    {
    public:

        static std::unique_ptr<IShader> MakeShader();
        static uint32_t GetPositionAttribute(IShader* shader);
        static uint32_t GetRotationAttribute(IShader* shader);
        static uint32_t GetColorAttribute(IShader* shader);
        static uint32_t GetPointSizeAttribute(IShader* shader);
    };
}
