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
#include "MonoPtrFwd.h"

#include <memory>
#include <unordered_map>

namespace editor
{
    class ImGuiShader;

    class ImGuiRenderer : public mono::IDrawable
    {
    public:

        ImGuiRenderer(const math::Vector2f& window_size);
        ImGuiRenderer(const math::Vector2f& window_size, const std::unordered_map<unsigned int, mono::ITexturePtr>& textures);
        virtual ~ImGuiRenderer();

        void SetWindowSize(const math::Vector2f& window_size);

    private:

        void Initialize();

        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual math::Quad BoundingBox() const;

        math::Vector2f m_windowSize;

        std::shared_ptr<editor::ImGuiShader> m_shader;
        std::unordered_map<unsigned int, mono::ITexturePtr> m_textures;
    };
}
