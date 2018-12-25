
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
        virtual void SetPointSize(float size);
        virtual unsigned int GetPositionAttributeLocation() const;
        virtual unsigned int GetColorAttributeLocation() const;

    private:

        unsigned int m_program;
        
        int m_mv_matrix_location;
        int m_p_matrix_location;
        int m_point_size_location;
        
        unsigned int m_position_attribute_location;
        unsigned int m_color_attribute_location;
    };
}
