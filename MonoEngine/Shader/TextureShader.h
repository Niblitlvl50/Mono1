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
        virtual void Clear();
        virtual unsigned int GetShaderId() const;
        virtual void LoadMatrices(const math::Matrix& projection, const math::Matrix& modelview);

        virtual int GetPositionAttributeLocation() const;
        virtual int GetTextureAttributeLocation() const;

        virtual void SetShade(const mono::Color& color);
        virtual void SetAlphaTexture(bool isAlpha);

    private:

        unsigned int mProgram;

        int mMVMatrixLocation;
        int mPMatrixLocation;

        int mPositionAttributeLocation;
        int mTextureAttributeLocation;

        int mSamplerLocation;
        int mColorShadeLocation;
        int mIsAlphaTextureLocation;
    };
}

