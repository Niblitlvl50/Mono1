//
//  ImGuiDrawable.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 20/06/16.
//
//

#pragma once

#include "IDrawable.h"
#include "Vector2f.h"
#include "Matrix.h"
#include "MonoPtrFwd.h"

#include <memory>

namespace editor
{
    class ImGuiShader;

    class ImGuiRenderer : public mono::IDrawable
    {
    public:

        ImGuiRenderer(const math::Vector2f& window_size);
        virtual ~ImGuiRenderer();

    private:

        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual math::Quad BoundingBox() const;

        math::Vector2f m_windowSize;
        math::Matrix m_projection;
        math::Matrix m_modelView;

        std::shared_ptr<editor::ImGuiShader> m_shader;
        mono::ITexturePtr m_texture;
    };
}
