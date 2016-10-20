//
//  ImGuiShader.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 19/06/16.
//
//

#pragma once

#include "Shader/IShader.h"

namespace editor
{
    class ImGuiShader : public mono::IShader
    {
    public:

        ImGuiShader();
        virtual ~ImGuiShader();

        virtual void Use();
        virtual unsigned int GetShaderId() const;
        virtual void LoadProjectionMatrix(const math::Matrix& projection);
        virtual void LoadModelViewMatrix(const math::Matrix& modelView);

        void SetTextureValue(int value);

        unsigned int PositionAttribute() const;
        unsigned int TextureAttribute() const;
        unsigned int ColorAttribute() const;

        int TextureLocation() const;

    private:

        unsigned int m_program;

        int m_MVMatrixLocation;
        int m_PMatrixLocation;

        unsigned int m_positionAttributeLocation;
        unsigned int m_textureAttributeLocation;
        unsigned int m_colorAttributeLocation;

        int m_textureLocation;
    };
}
