//
//  ColorShader.h
//  MonoOSX
//
//  Created by Niklas Damberg on 25/12/14.
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

    class ColorShader
    {
    public:

        ColorShader();

        int GetMVMatrixLocation() const;
        int GetPMatrixLocation() const;

        int GetPositionAttributeLocation() const;
        int GetColorAttributeLocation() const;

        void LoadMatrices(const math::Matrix& projection, const math::Matrix& modelview);

        void Use();
        void Clear();

    private:

        std::shared_ptr<mono::IShader> mShader;
        
        int mMVMatrixLocation;
        int mPMatrixLocation;
        int mPositionAttributeLocation;
        int mColorAttributeLocation;
    };
}
