//
//  IColorShader.h
//  MonoOSX
//
//  Created by Niklas Damberg on 26/12/14.
//
//

#pragma once

#include "IShader.h"

namespace mono
{
    class IColorShader : public IShader
    {
    public:

        virtual ~IColorShader()
        { }

        virtual int GetPositionAttributeLocation() const = 0;
        virtual int GetColorAttributeLocation() const = 0;
    };
}

