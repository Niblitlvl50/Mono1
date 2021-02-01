
#pragma once

#include "MonoFwd.h"
#include "Rendering/RenderFwd.h"
#include "Rendering/IDrawable.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>

namespace mono
{
    struct SpriteDrawBuffers
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> offsets;
        std::unique_ptr<IRenderBuffer> uv;
        std::unique_ptr<IRenderBuffer> uv_flipped;
        std::unique_ptr<IRenderBuffer> heights;
        int vertices_per_sprite;
    };

    struct SpriteShadowBuffers
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> colors;
        std::unique_ptr<IElementBuffer> indices;
    };

    class SpriteBatchDrawer : public mono::IDrawable
    {
    public:

        SpriteBatchDrawer(const mono::TransformSystem* transform_system, mono::SpriteSystem* sprite_system);
        ~SpriteBatchDrawer();

        void PreloadSpriteData(const std::vector<std::string>& sprite_files);
    
    private:

        void BuildSpriteDrawBuffers(const mono::SpriteData* sprite_data) const;
        void BuildSpriteShadowBuffers(const mono::ISprite* sprite, uint32_t id) const;

        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        const mono::TransformSystem* m_transform_system;
        mono::SpriteSystem* m_sprite_system;

        std::unique_ptr<mono::IElementBuffer> m_indices;
        mutable std::unordered_map<uint32_t, SpriteDrawBuffers> m_sprite_buffers;
        mutable std::unordered_map<uint32_t, SpriteShadowBuffers> m_shadow_buffers;
    };
}
