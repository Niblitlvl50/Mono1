
#pragma once

#include "Rendering/IDrawable.h"
#include "Rendering/RenderPtrFwd.h"
#include "Math/Vector.h"

#include <memory>
#include <unordered_map>

class ImGuiRenderer : public mono::IDrawable
{
public:

    ImGuiRenderer(const char* config_file, const math::Vector& window_size);
    ImGuiRenderer(
        const char* config_file, const math::Vector& window_size, const std::unordered_map<uint32_t, mono::ITexturePtr>& textures);
    virtual ~ImGuiRenderer();

    void SetWindowSize(const math::Vector& window_size);

private:

    void Initialize(const char* config_file);
    void doDraw(mono::IRenderer& renderer) const override;
    math::Quad BoundingBox() const override;

    math::Vector m_window_size;
    std::unique_ptr<class ImGuiShader> m_shader;
    std::unordered_map<uint32_t, mono::ITexturePtr> m_textures;
};
