//
//  TextureShader.h
//  MonoOSX
//
//  Created by Niklas Damberg on 26/12/14.
//
//

#pragma once

#include <memory>

namespace math
{
    struct Matrix;
}

namespace mono
{
    class IShader;
    struct Color;
    
    class TextureShader
    {
    public:

        TextureShader();

        int GetMVMatrixLocation() const;
        int GetPMatrixLocation() const;

        int GetPositionAttributeLocation() const;
        int GetTextureAttributeLocation() const;

        void LoadMatrices(const math::Matrix& projection, const math::Matrix& modelview);
        void SetShade(const mono::Color& shade);

        void Use();
        void Clear();

    private:

        std::shared_ptr<mono::IShader> mShader;

        int mMVMatrixLocation;
        int mPMatrixLocation;
        int mSamplerLocation;
        int mColorShadeLocation;
        
        int mPositionAttributeLocation;
        int mTextureAttributeLocation;
    };
}

