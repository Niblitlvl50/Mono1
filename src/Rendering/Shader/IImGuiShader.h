
#pragma once

#include "IShader.h"

namespace mono
{
    class IImGuiShader : public mono::IShader
    {
    public:

        virtual ~IImGuiShader() = default;

        virtual void SetTextureValue(int value) = 0;
        virtual unsigned int PositionAttribute() const = 0;
        virtual unsigned int TextureAttribute() const = 0;
        virtual unsigned int ColorAttribute() const = 0;
        virtual int TextureLocation() const = 0;
    };
}
