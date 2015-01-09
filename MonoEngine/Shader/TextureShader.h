//
//  TextureShader.h
//  MonoOSX
//
//  Created by Niklas Damberg on 26/12/14.
//
//

#pragma once

#include "ITextureShader.h"

namespace mono
{
    class TextureShader : public ITextureShader
    {
    public:

        TextureShader();
        ~TextureShader();

        virtual void Use();
        virtual unsigned int GetShaderId() const;
        virtual void LoadProjectionMatrix(const math::Matrix& projection);
        virtual void LoadModelViewMatrix(const math::Matrix& modelView);

        virtual unsigned int GetPositionAttributeLocation() const;
        virtual unsigned int GetTextureAttributeLocation() const;

        virtual void SetShade(const mono::Color& color);
        virtual void SetAlphaTexture(bool isAlpha);

    private:

        unsigned int mProgram;

        int mMVMatrixLocation;
        int mPMatrixLocation;

        unsigned int mPositionAttributeLocation;
        unsigned int mTextureAttributeLocation;

        int mSamplerLocation;
        int mColorShadeLocation;
        int mIsAlphaTextureLocation;
    };
}

