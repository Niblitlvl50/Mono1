
#pragma once

#include "IPointSpriteShader.h"

namespace mono
{
    class PointSpriteShader : public IPointSpriteShader
    {
    public:

        PointSpriteShader();

        void Use() override;
        unsigned int GetShaderId() const override;
        void LoadProjectionMatrix(const math::Matrix& projection) override;
        void LoadModelViewMatrix(const math::Matrix& modelView) override;

        unsigned int GetPositionAttribute() const override;
        unsigned int GetRotationAttribute() const override;
        unsigned int GetColorAttribute() const override;
        unsigned int GetPointSizeAttribute() const override;

    private:

        unsigned int m_program;
        
        int m_mv_matrix_location;
        int m_p_matrix_location;
        
        unsigned int m_position_attribute;
        unsigned int m_rotation_attribute;
        unsigned int m_color_attribute;
        unsigned int m_point_size_attribute;
    };
}
