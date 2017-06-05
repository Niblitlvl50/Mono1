
#pragma once

#include "Rendering/IDrawable.h"
#include "Rendering/RenderPtrFwd.h"
#include "Math/Vector.h"

#include <memory>
#include <unordered_map>

class ImGuiShader;

class ImGuiRenderer : public mono::IDrawable
{
public:

    ImGuiRenderer(const char* config_file,
                  const math::Vector& window_size);

    ImGuiRenderer(const char* config_file,
                  const math::Vector& window_size,
                  const std::unordered_map<unsigned int, mono::ITexturePtr>& textures);

    virtual ~ImGuiRenderer();

    void SetWindowSize(const math::Vector& window_size);

private:

    void Initialize(const char* config_file);

    virtual void doDraw(mono::IRenderer& renderer) const;
    virtual math::Quad BoundingBox() const;

    math::Vector m_windowSize;

    std::shared_ptr<ImGuiShader> m_shader;
    std::unordered_map<unsigned int, mono::ITexturePtr> m_textures;
};
