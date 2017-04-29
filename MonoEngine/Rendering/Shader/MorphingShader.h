
#pragma once

#include "IMorphingShader.h"

namespace mono
{
    class MorphingShader : public IMorphingShader
    {
    public:

        MorphingShader();

        virtual void SetMorphGrade(float grade);
        virtual unsigned int GetPositionLocation() const;
        virtual unsigned int GetMorphPositionLocation() const;
        virtual unsigned int GetColorLocation() const;

        virtual void Use();
        virtual unsigned int GetShaderId() const;
        virtual void LoadProjectionMatrix(const math::Matrix& projection);
        virtual void LoadModelViewMatrix(const math::Matrix& modelView);

        unsigned int m_program;

        int m_MVMatrixLocation;
        int m_PMatrixLocation;
        int m_morphGrade;

        unsigned int m_positionLocation;
        unsigned int m_morphPositionLocation;
        unsigned int m_colorLocation;
    };
}
