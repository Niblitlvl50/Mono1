
#pragma once

#include "IImGuiShader.h"

namespace mono
{
    class ImGuiShader : public mono::IImGuiShader
    {
    public:

        ImGuiShader();
        virtual ~ImGuiShader();

        void Use() override;
        unsigned int GetShaderId() const override;
        void LoadProjectionMatrix(const math::Matrix& projection) override;
        void LoadModelViewMatrix(const math::Matrix& modelview) override;

        void SetTextureValue(int value) override;
        unsigned int PositionAttribute() const override;
        unsigned int TextureAttribute() const override;
        unsigned int ColorAttribute() const override;
        int TextureLocation() const override;

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
