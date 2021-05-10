
#include "SpriteBatchDrawer.h"

#include "ISprite.h"
#include "SpriteSystem.h"
#include "SpriteProperties.h"

#include "Rendering/RenderSystem.h"
#include "Rendering/IRenderer.h"
#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Rendering/Sprite/ISpriteFactory.h"

#include "TransformSystem/TransformSystem.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"

#include <algorithm>

using namespace mono;

namespace
{
    struct SpriteTransformPair
    {
        uint32_t entity_id;
        math::Matrix transform;
        math::Quad world_bb;
        mono::ISprite* sprite;
        int layer;
    };
}

SpriteBatchDrawer::SpriteBatchDrawer(const mono::TransformSystem* transform_system, mono::SpriteSystem* sprite_system)
    : m_transform_system(transform_system)
    , m_sprite_system(sprite_system)
{
    constexpr uint16_t indices[] = {
        0, 1, 2, 0, 2, 3
    };
    m_indices = mono::CreateElementBuffer(mono::BufferType::STATIC, 6, indices);
}

SpriteBatchDrawer::~SpriteBatchDrawer()
{ }

void SpriteBatchDrawer::PreloadSpriteData(const std::vector<std::string>& sprite_files)
{
    for(const std::string& file : sprite_files)
    {
        const mono::SpriteData* sprite_data = mono::GetSpriteFactory()->GetSpriteDataForFile(file.c_str());
        if(!sprite_data)
            continue; // Error

        m_sprite_buffers[sprite_data->hash] = BuildSpriteDrawBuffers(sprite_data);
    }
}

void SpriteBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    std::vector<SpriteTransformPair> sprites_to_draw;
    sprites_to_draw.reserve(128);

    const auto collect_sprites = [this, &renderer, &sprites_to_draw](mono::ISprite* sprite, int layer, uint32_t id)
    {
        if(!sprite->GetTexture())
            return;

        const math::Quad& world_bounds = m_transform_system->GetWorldBoundingBox(id);
        if(renderer.Cull(world_bounds))
        {
            const uint32_t sprite_hash = sprite->GetSpriteHash();
            auto it = m_sprite_buffers.find(sprite_hash);
            if(it == m_sprite_buffers.end())
                m_sprite_buffers[sprite_hash] = BuildSpriteDrawBuffers(sprite->GetSpriteData());

            const bool has_shadow = (sprite->GetProperties() & mono::SpriteProperty::SHADOW);
            if(has_shadow)
            {
                auto shadow_it = m_shadow_buffers.find(id);
                if(shadow_it == m_shadow_buffers.end())
                    m_shadow_buffers[id] = BuildSpriteShadowBuffers(sprite);
            }

            const math::Matrix& transform = m_transform_system->GetWorld(id);
            sprites_to_draw.push_back({ id, transform, world_bounds, sprite, layer });
        }
    };

    m_sprite_system->ForEachSprite(collect_sprites);

    const auto sort_on_y_and_layer = [](const SpriteTransformPair& first, const SpriteTransformPair& second)
    {
        if(first.layer == second.layer)
            return math::Bottom(first.world_bb) > math::Bottom(second.world_bb);

        return first.layer < second.layer;
    };

    std::sort(sprites_to_draw.begin(), sprites_to_draw.end(), sort_on_y_and_layer);

    for(const SpriteTransformPair& sprite_transform : sprites_to_draw)
    {
        const math::Matrix& world_transform = renderer.GetTransform() * sprite_transform.transform;
        auto transform_scope = mono::MakeTransformScope(world_transform, &renderer);

        mono::ISprite* sprite = sprite_transform.sprite;

        const bool has_shadow = (sprite->GetProperties() & mono::SpriteProperty::SHADOW);
        if(has_shadow)
        {
            const auto it = m_shadow_buffers.find(sprite_transform.entity_id);
            if(it != m_shadow_buffers.end())
            {
                const SpriteShadowBuffers& shadow_buffers = it->second;
                renderer.DrawTrianges(
                    shadow_buffers.vertices.get(),
                    shadow_buffers.colors.get(),
                    shadow_buffers.indices.get(),
                    0,
                    shadow_buffers.indices->Size());
            }
        }

        const SpriteDrawBuffers& sprite_buffers = m_sprite_buffers[sprite->GetSpriteHash()];
        const int offset = sprite->GetCurrentFrameIndex() * sprite_buffers.vertices_per_sprite;
        mono::ITexture* texture = sprite->GetTexture();

        renderer.DrawSprite(
            sprite_transform.sprite,
            sprite_buffers.vertices.get(),
            sprite_buffers.offsets.get(),
            sprite_buffers.uv.get(),
            sprite_buffers.uv_flipped.get(),
            sprite_buffers.heights.get(),
            m_indices.get(),
            texture,
            offset);
    }
}

math::Quad SpriteBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
