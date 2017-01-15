
#pragma once

#include "IPointSpriteShader.h"

namespace mono
{
    class PointSpriteShader : public IPointSpriteShader
    {
    public:

        PointSpriteShader();

        virtual void Use();
        virtual unsigned int GetShaderId() const;
        virtual void LoadProjectionMatrix(const math::Matrix& projection);
        virtual void LoadModelViewMatrix(const math::Matrix& modelView);
        virtual void SetPointSize(float size);
        virtual unsigned int GetPositionAttributeLocation() const;
        virtual unsigned int GetColorAttributeLocation() const;

    private:

        unsigned int mProgram;
        
        int mMVMatrixLocation;
        int mPMatrixLocation;
        int m_pointSizeLocation;
        
        unsigned int mPositionAttributeLocation;
        unsigned int mColorAttributeLocation;
    };
}
