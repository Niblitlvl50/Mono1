//
//  ImGuiDrawable.hpp
//  MonoiOS
//
//  Created by Niklas Damberg on 20/06/16.
//
//

#pragma once

#include "IDrawable.h"
#include "Math/Vector2f.h"
#include "MonoPtrFwd.h"

#include <memory>
#include <unordered_map>

class ImGuiShader;

class ImGuiRenderer : public mono::IDrawable
{
public:

    ImGuiRenderer(const char* config_file,
                  const math::Vector2f& window_size);

    ImGuiRenderer(const char* config_file,
                  const math::Vector2f& window_size,
                  const std::unordered_map<unsigned int, mono::ITexturePtr>& textures);

    virtual ~ImGuiRenderer();

    void SetWindowSize(const math::Vector2f& window_size);

private:

    void Initialize(const char* config_file);

    virtual void doDraw(mono::IRenderer& renderer) const;
    virtual math::Quad BoundingBox() const;

    math::Vector2f m_windowSize;

    std::shared_ptr<ImGuiShader> m_shader;
    std::unordered_map<unsigned int, mono::ITexturePtr> m_textures;
};
