
#pragma once

#include "Rendering/IDrawable.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/Texture/ITextureFactory.h"

#include <vector>

#define ENUM_BIT(n) (1 << (n))

namespace mono
{
    enum TextureModeFlags
    {
        REPEAT      = ENUM_BIT(0),
        STRETCH     = ENUM_BIT(1),
        BLUR        = ENUM_BIT(2),
    };

    class StaticBackground : public mono::IDrawable
    {
    public:

        StaticBackground();
        StaticBackground(const char* texture_filename, uint32_t texture_mode_flags);
        StaticBackground(const std::vector<math::Vector>& vertices, const char* texture_filename, uint32_t texture_mode_flags);

        void Load(const char* texture_filename, uint32_t texture_mode_flags);
        void Load(const std::vector<math::Vector>& vertices, const char* background_texture, uint32_t texture_mode_flags);

        void Clear();

        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        TextureModeFlags m_mode;
        std::unique_ptr<mono::IRenderBuffer> m_vertex_buffer;
        std::unique_ptr<mono::IRenderBuffer> m_texture_buffer;
        std::unique_ptr<mono::IElementBuffer> m_index_buffer;
        mono::ITexturePtr m_texture;
    };
}
