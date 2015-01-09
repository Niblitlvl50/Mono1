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
        virtual unsigned int GetShaderId() const;
        virtual void LoadProjectionMatrix(const math::Matrix& projection);
        virtual void LoadModelViewMatrix(const math::Matrix& modelView);

        virtual unsigned int GetPositionAttributeLocation() const;
        virtual unsigned int GetColorAttributeLocation() const;

    private:

        unsigned int mProgram;
        
        int mMVMatrixLocation;
        int mPMatrixLocation;
        
        unsigned int mPositionAttributeLocation;
        unsigned int mColorAttributeLocation;
    };
}
