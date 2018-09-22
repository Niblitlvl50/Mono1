
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

        virtual void SetShade(const mono::Color::RGBA& color);
        virtual void SetAlphaTexture(bool isAlpha);
        virtual void SetTextureOffset(float offset);

    private:

        unsigned int m_program;

        int m_MV_matrix_location;
        int m_P_matrix_location;

        unsigned int m_position_attribute_location;
        unsigned int m_texture_attribute_location;

        int m_color_shade_location;
        int m_is_alpha_texture_location;
        int m_texture_offset_location;
    };
}

