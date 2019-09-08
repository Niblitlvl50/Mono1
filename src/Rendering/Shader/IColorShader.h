
#pragma once

#include "IShader.h"

namespace mono
{
    class IColorShader : public IShader
    {
    public:

        virtual unsigned int GetPositionAttributeLocation() const = 0;
        virtual unsigned int GetColorAttributeLocation() const = 0;
        virtual void SetPointSize(float size) = 0;
    };
}

