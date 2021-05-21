
#pragma once

#include "MonoFwd.h"
#include "Rendering/IDrawable.h"
#include "SpriteBufferFactory.h"
#include "Math/Vector.h"
#include "Rendering/Texture/ITextureFactory.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

namespace mono
{
    class SpriteBatchDrawer : public mono::IDrawable
    {
    public:

        SpriteBatchDrawer(const mono::TransformSystem* transform_system, mono::SpriteSystem* sprite_system);
        ~SpriteBatchDrawer();

        void PreloadSpriteData(const std::vector<std::string>& sprite_files);

    private:

        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        const mono::TransformSystem* m_transform_system;
        mono::SpriteSystem* m_sprite_system;

        std::unique_ptr<mono::IElementBuffer> m_sprite_indices;
        mono::ITexturePtr m_shadow_texture;

        mutable std::unordered_map<uint32_t, SpriteDrawBuffers> m_sprite_buffers;

        struct ShadowData
        {
            math::Vector offset;
            float size;
        };
        mutable std::unordered_map<uint32_t, ShadowData> m_shadow_data_cache;
        mutable std::unordered_map<uint32_t, SpriteShadowBuffers> m_shadow_buffers;
    };
}
