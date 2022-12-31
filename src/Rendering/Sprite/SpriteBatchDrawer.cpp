
#include "SpriteBatchDrawer.h"

#include "ISprite.h"
#include "SpriteSystem.h"
#include "SpriteProperties.h"

#include "Rendering/RenderSystem.h"
#include "Rendering/IRenderer.h"
#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Rendering/Sprite/ISpriteFactory.h"
#include "Rendering/Texture/ITextureFactory.h"

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

    struct ShadowDrawData
    {
        uint32_t entity_id;
        math::Matrix transform;
        mono::ISprite* sprite;
    };
}

SpriteBatchDrawer::SpriteBatchDrawer(
    const mono::TransformSystem* transform_system, mono::SpriteSystem* sprite_system, const mono::RenderSystem* render_system)
    : m_transform_system(transform_system)
    , m_sprite_system(sprite_system)
    , m_render_system(render_system)
{
    constexpr uint16_t indices[] = {
        0, 1, 2, 0, 2, 3
    };
    m_sprite_indices = mono::CreateElementBuffer(mono::BufferType::STATIC, 6, indices);

    const char* sprite_shadow_texture = mono::SpriteShadowTexture();
    if(sprite_shadow_texture)
        m_shadow_texture = mono::GetTextureFactory()->CreateTexture(sprite_shadow_texture);
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

void SpriteBatchDrawer::ReloadSpriteData(uint32_t sprite_hash)
{
    m_sprite_buffers.erase(sprite_hash);
}

void SpriteBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    std::vector<SpriteTransformPair> sprites_to_draw;
    std::vector<ShadowDrawData> shadows_to_draw;

    sprites_to_draw.reserve(128);
    shadows_to_draw.reserve(128);

    const auto collect_sprites = [&, this](uint32_t id, mono::ISprite& sprite)
    {
        if(!sprite.GetTexture())
            return;

        const math::Matrix& transform = m_transform_system->GetWorld(id);
        const math::Quad world_bounds = m_transform_system->GetWorldBoundingBox(id);

        if(renderer.Cull(world_bounds))
        {
            const uint32_t sprite_hash = sprite.GetSpriteHash();
            auto it = m_sprite_buffers.find(sprite_hash);
            if(it == m_sprite_buffers.end())
                m_sprite_buffers[sprite_hash] = BuildSpriteDrawBuffers(sprite.GetSpriteData());

            const float sort_offset = m_render_system->GetRenderSortOffsetOrDefault(id);
            math::Quad world_bounds_offseted = world_bounds;
            world_bounds_offseted.bottom_left.y += sort_offset;

            const int render_layer = m_render_system->GetRenderLayerOrDefault(id);
            sprites_to_draw.push_back({ id, transform, world_bounds_offseted, &sprite, render_layer });
        }

        const bool has_shadow = (sprite.GetProperties() & mono::SpriteProperty::SHADOW);
        if(has_shadow)
        {
            const math::Vector& shadow_offset = sprite.GetShadowOffset();
            const float shadow_radius = sprite.GetShadowSize();
            const math::Quad shadow_bb = math::Quad(math::Center(world_bounds) + shadow_offset, shadow_radius * 2.0f, shadow_radius);

            const bool is_shadow_visible = renderer.Cull(shadow_bb);
            if(is_shadow_visible)
            {
                bool need_update = false;

                auto shadow_it = m_shadow_buffers.find(id);
                if(shadow_it != m_shadow_buffers.end())
                {
                    auto shadow_cached_it = m_shadow_data_cache.find(id);
                    need_update = 
                        (shadow_cached_it->second.offset != shadow_offset) ||
                        (shadow_cached_it->second.radius != shadow_radius);
                }
                else
                {
                    need_update = true;
                }

                if(need_update)
                {
                    m_shadow_data_cache[id] = { shadow_offset, shadow_radius };
                    m_shadow_buffers[id] = BuildSpriteShadowBuffers(shadow_offset, shadow_radius);
                }

                shadows_to_draw.push_back({ id, transform, &sprite });
            }
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

    if(m_shadow_texture)
    {
        for(const ShadowDrawData& shadow_draw : shadows_to_draw)
        {
            const math::Matrix& world_transform = renderer.GetTransform() * shadow_draw.transform;
            auto transform_scope = mono::MakeTransformScope(world_transform, &renderer);

            const auto it = m_shadow_buffers.find(shadow_draw.entity_id);
            if(it != m_shadow_buffers.end())
            {
                const SpriteShadowBuffers& shadow_buffers = it->second;
                renderer.DrawGeometry(
                    shadow_buffers.vertices.get(),
                    shadow_buffers.uv.get(),
                    m_sprite_indices.get(),
                    m_shadow_texture.get(),
                    mono::Color::WHITE,
                    false,
                    m_sprite_indices->Size());
            }
        }
    }

    for(const SpriteTransformPair& sprite_transform : sprites_to_draw)
    {
        const math::Matrix& world_transform = renderer.GetTransform() * sprite_transform.transform;
        auto transform_scope = mono::MakeTransformScope(world_transform, &renderer);

        mono::ISprite* sprite = sprite_transform.sprite;

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
            m_sprite_indices.get(),
            texture,
            offset);
    }
}

math::Quad SpriteBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
