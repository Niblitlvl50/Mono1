
#pragma once

#include "Math/MathFwd.h"

namespace mono
{
    class IShader
    {
    public:

        virtual ~IShader() = default;
        virtual void Use() = 0;
        virtual unsigned int GetShaderId() const = 0;
        virtual void LoadProjectionMatrix(const math::Matrix& projection) = 0;
        virtual void LoadModelViewMatrix(const math::Matrix& modelView) = 0;
    };
}
