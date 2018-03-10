
#pragma once

#include "IPointSpriteShader.h"

namespace mono
{
    class PointSpriteShader : public IPointSpriteShader
    {
    public:

        PointSpriteShader();

        virtual void Use() override;
        virtual unsigned int GetShaderId() const override;
        virtual void LoadProjectionMatrix(const math::Matrix& projection) override;
        virtual void LoadModelViewMatrix(const math::Matrix& modelView) override;

        virtual unsigned int GetPositionAttribute() const override;
        virtual unsigned int GetColorAttribute() const override;
        virtual unsigned int GetPointSizeAttribute() const override;

    private:

        unsigned int m_program;
        
        int m_mv_matrix_location;
        int m_p_matrix_location;
        
        unsigned int m_position_attribute;
        unsigned int m_color_attribute;
        unsigned int m_point_size_attribute;
    };
}
