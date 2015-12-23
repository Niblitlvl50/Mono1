//
//  IMorphingShader.h
//  MonoOSX
//
//  Created by Niklas Damberg on 21/12/15.
//
//

#pragma once

#include "IShader.h"

namespace mono
{
    class IMorphingShader : public IShader
    {
    public:

        virtual ~IMorphingShader()
        { }

        virtual void SetMorphGrade(float grade) = 0;

        virtual unsigned int GetPositionLocation() const = 0;
        virtual unsigned int GetMorphPositionLocation() const = 0;
        virtual unsigned int GetColorLocation() const = 0;
    };
}

