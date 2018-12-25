
#pragma once

#include "IShader.h"

namespace mono
{
    class IPointSpriteShader : public IShader
    {
    public:

        virtual ~IPointSpriteShader()
        { }

        virtual unsigned int GetPositionAttribute() const = 0;
        virtual unsigned int GetRotationAttribute() const = 0;
        virtual unsigned int GetColorAttribute() const = 0;
        virtual unsigned int GetPointSizeAttribute() const = 0;
    };
}
