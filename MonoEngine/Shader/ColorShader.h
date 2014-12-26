//
//  ColorShader.h
//  MonoOSX
//
//  Created by Niklas Damberg on 25/12/14.
//
//

#pragma once

#include "IColorShader.h"

namespace mono
{
    class ColorShader : public IColorShader
    {
    public:

        ColorShader();

        virtual void Use();
        virtual void Clear();
        virtual unsigned int GetShaderId() const;
        virtual void LoadMatrices(const math::Matrix& projection, const math::Matrix& modelview);

        virtual int GetPositionAttributeLocation() const;
        virtual int GetColorAttributeLocation() const;

    private:

        unsigned int mProgram;
        
        int mMVMatrixLocation;
        int mPMatrixLocation;
        
        int mPositionAttributeLocation;
        int mColorAttributeLocation;
    };
}
