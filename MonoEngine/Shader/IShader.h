//
//  IShader.h
//  MonoOSX
//
//  Created by Niklas Damberg on 21/12/14.
//
//

#pragma once

#include "MathFwd.h"

namespace mono
{
    class IShader
    {
    public:

        virtual ~IShader()
        { }

        virtual void Use() = 0;
        virtual unsigned int GetShaderId() const = 0;
        virtual void LoadProjectionMatrix(const math::Matrix& projection) = 0;
        virtual void LoadModelViewMatrix(const math::Matrix& modelView) = 0;
    };
}
