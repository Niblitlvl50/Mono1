
#pragma once

#include <memory>
#include <cstdint>

namespace mono
{
    class IShader;

    class ColorShader
    {
    public:

        static std::unique_ptr<IShader> MakeShader();
        static void SetPointSize(IShader* shader, float size);
        static uint32_t GetPositionAttribute(IShader* shader);
        static uint32_t GetColorAttribute(IShader* shader);
    };
}
