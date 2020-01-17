
#pragma once

#include <memory>
#include <cstdint>

namespace mono
{
    class IShader;

    class ImGuiShader
    {
    public:

        static std::unique_ptr<IShader> MakeShader();
        static void SetTextureValue(IShader* shader, int value);
        static uint32_t GetPositionAttribute(IShader* shader);
        static uint32_t GetTextureAttribute(IShader* shader);
        static uint32_t GetColorAttribute(IShader* shader);
        static uint32_t GetTextureLocation(IShader* shader);
    };
}
