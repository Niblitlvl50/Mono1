
#include "SpriteBatchDrawer.h"

#include "ISprite.h"
#include "SpriteSystem.h"
#include "SpriteProperties.h"

#include "Rendering/RenderSystem.h"
#include "Rendering/IRenderer.h"
#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Rendering/Sprite/ISpriteFactory.h"
#include "Rendering/Sprite/SpriteData.h"
#include "Rendering/Texture/ITextureFactory.h"

#include "TransformSystem/TransformSystem.h"
#include "Math/Quad.h"
#include "Math/MathFunctions.h"

#include <algorithm>
#include <cmath>
#include <iterator>

using namespace mono;

namespace
{
    struct SpriteDrawData
    {
        math::Matrix transform;
        float sort_y;
        mono::ISprite* sprite;
        int layer;
    };

    struct ShadowDrawData
    {
        uint32_t entity_id;
        math::Matrix transform;
    };

    // Calls draw_func with the transform and frame index of the animation facing the entity's heading.
    template <typename T>
    void DrawDirectional(const SpriteDrawData& draw_data, int vertices_per_sprite, T&& draw_func)
    {
        static const char* direction_names[] = {
            "east", "north_east", "north", "north_west", "west", "south_west", "south", "south_east"
        };

        // The entity's local x axis is its heading.
        const float heading = math::ToDegrees(std::atan2(draw_data.transform.data[1], draw_data.transform.data[0]));
        const int sector = int(std::floor((heading + 22.5f) / 45.0f));
        const char* direction_name = direction_names[((sector % 8) + 8) % 8];

        const mono::SpriteData* sprite_data = draw_data.sprite->GetSpriteData();
        for(const mono::SpriteAnimation& animation : sprite_data->animations)
        {
            if(animation.name != direction_name || animation.frames.empty())
                continue;

            const int frame = draw_data.sprite->GetActiveAnimationFrame() % int(animation.frames.size());
            const math::Matrix unrotated = math::CreateMatrixWithPosition(math::GetPosition(draw_data.transform));
            draw_func(unrotated, animation.frames[frame].frame * vertices_per_sprite);
            return;
        }
    }

    // Calls draw_func with the model transform and frame index of each slice, from the bottom up.
    template <typename T>
    void ForEachSlice(const SpriteDrawData& draw_data, int vertices_per_sprite, T&& draw_func)
    {
        const mono::ISprite* sprite = draw_data.sprite;
        if(sprite->HasProperty(mono::SpriteProperty::DIRECTIONAL))
        {
            DrawDirectional(draw_data, vertices_per_sprite, draw_func);
            return;
        }

        if(!sprite->HasProperty(mono::SpriteProperty::STACKED))
        {
            draw_func(draw_data.transform, sprite->GetCurrentFrameIndex() * vertices_per_sprite);
            return;
        }

        const mono::SpriteData* sprite_data = sprite->GetSpriteData();
        const int animation_id = sprite->GetActiveAnimation();
        if(animation_id < 0 || animation_id >= int(sprite_data->animations.size()))
            return;

        // Up on screen, not rotated with the entity.
        const float slice_height = 1.0f / mono::RenderSystem::PixelsPerMeter();

        const std::vector<mono::SpriteAnimationFrame>& slices = sprite_data->animations[animation_id].frames;
        for(size_t index = 0; index < slices.size(); ++index)
        {
            const math::Matrix slice_offset = math::CreateMatrixWithPosition(math::Vector(0.0f, slice_height * float(index)));
            draw_func(slice_offset * draw_data.transform, slices[index].frame * vertices_per_sprite);
        }
    }
}

SpriteBatchDrawer::SpriteBatchDrawer(
    const mono::TransformSystem* transform_system, mono::SpriteSystem* sprite_system, mono::RenderSystem* render_system)
    : m_transform_system(transform_system)
    , m_sprite_system(sprite_system)
    , m_render_system(render_system)
{
    constexpr uint16_t indices[] = {
        0, 1, 2, 0, 2, 3
    };
    m_sprite_indices = mono::CreateElementBuffer(mono::BufferType::STATIC, 6, indices, "sprite_batch_drawer");

    const char* sprite_shadow_texture = mono::RenderSystem::SpriteShadowTexture();
    if(sprite_shadow_texture)
        m_shadow_texture = mono::RenderSystem::GetTextureFactory()->CreateTexture(sprite_shadow_texture);
}

SpriteBatchDrawer::~SpriteBatchDrawer()
{ }

void SpriteBatchDrawer::PreloadSpriteData(const std::vector<std::string>& sprite_files)
{
    for(const std::string& file : sprite_files)
    {
        const mono::SpriteData* sprite_data = mono::RenderSystem::GetSpriteFactory()->GetSpriteDataForFile(file.c_str());
        if(!sprite_data)
            continue; // Error

        m_sprite_buffers[sprite_data->hash] = BuildSpriteDrawBuffers(sprite_data, "sprite_batch_drawer");
    }
}

void SpriteBatchDrawer::ReloadSpriteData(uint32_t sprite_hash)
{
    m_sprite_buffers.erase(sprite_hash);
}

void SpriteBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    std::vector<SpriteDrawData> sprites_to_draw;
    std::vector<ShadowDrawData> shadows_to_draw;

    sprites_to_draw.reserve(128);
    shadows_to_draw.reserve(128);

    std::vector<uint32_t> active_sprites;

    const auto collect_sprites = [&, this](uint32_t id, mono::ISprite& sprite)
    {
        if(!sprite.GetTexture())
            return;

        const math::Matrix& transform = m_transform_system->GetWorld(id);
        const math::Quad world_bounds = m_transform_system->GetWorldBoundingBox(id);

        // Asked before culling, so an entity using its initial y records where it spawned, not where it first came into view.
        const float sort_y = m_render_system->GetSortPosition(id, math::Bottom(world_bounds));

        const bool sprite_is_visible = (renderer.Cull(world_bounds) == mono::CullResult::IN_VIEW);
        if(sprite_is_visible)
        {
            const uint32_t sprite_hash = sprite.GetSpriteHash();
            auto it = m_sprite_buffers.find(sprite_hash);
            if(it == m_sprite_buffers.end())
                m_sprite_buffers[sprite_hash] = BuildSpriteDrawBuffers(sprite.GetSpriteData(), "sprite_buffer-sprite_batch_drawer");

            const int render_layer = m_render_system->GetRenderLayerOrDefault(id);
            sprites_to_draw.push_back({ transform, sort_y, &sprite, render_layer });
        }

        bool shadow_is_visible = false;

        const bool has_shadow = sprite.HasProperty(mono::SpriteProperty::SHADOW);
        if(has_shadow && m_shadow_texture)
        {
            const math::Vector& shadow_offset = sprite.GetShadowOffset();
            const float shadow_radius = sprite.GetShadowSize();
            const math::Quad shadow_bb = math::Quad(math::Center(world_bounds) + shadow_offset, shadow_radius * 2.0f, shadow_radius);

            shadow_is_visible = (renderer.Cull(shadow_bb) == mono::CullResult::IN_VIEW);
            if(shadow_is_visible)
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

                shadows_to_draw.push_back({ id, transform });
            }
        }

        if(sprite_is_visible || shadow_is_visible)
            active_sprites.push_back(id);
    };
    m_sprite_system->ForEachSprite(collect_sprites);

    const auto sort_on_y_and_layer = [](const SpriteDrawData& first, const SpriteDrawData& second)
    {
        if(first.layer == second.layer)
            return first.sort_y > second.sort_y;

        return first.layer < second.layer;
    };
    std::sort(sprites_to_draw.begin(), sprites_to_draw.end(), sort_on_y_and_layer);

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

    for(const SpriteDrawData& draw_data : sprites_to_draw)
    {
        const SpriteDrawBuffers& sprite_buffers = m_sprite_buffers[draw_data.sprite->GetSpriteHash()];
        ForEachSlice(draw_data, sprite_buffers.vertices_per_sprite, [&](const math::Matrix& transform, int offset) {
            const math::Matrix& world_transform = renderer.GetTransform() * transform;
            auto transform_scope = mono::MakeTransformScope(world_transform, &renderer);
            renderer.DrawSprite(draw_data.sprite, &sprite_buffers, offset);
        });
    }

    // Draw outline
    for(const SpriteDrawData& draw_data : sprites_to_draw)
    {
        const bool draw_outline = draw_data.sprite->ShouldOutlineSprite();
        if(!draw_outline)
            continue;

        const SpriteDrawBuffers& sprite_buffers = m_sprite_buffers[draw_data.sprite->GetSpriteHash()];
        ForEachSlice(draw_data, sprite_buffers.vertices_per_sprite, [&](const math::Matrix& transform, int offset) {
            const math::Matrix& world_transform = renderer.GetTransform() * transform;
            auto transform_scope = mono::MakeTransformScope(world_transform, &renderer);
            renderer.DrawSpriteOutline(draw_data.sprite, &sprite_buffers, offset);
        });
    }


    // CLEAN UP

    std::vector<uint32_t> diff_result;
    std::set_difference(
        m_last_active_sprites.begin(),
        m_last_active_sprites.end(),
        active_sprites.begin(),
        active_sprites.end(),
        std::back_inserter(diff_result));

    m_last_active_sprites = active_sprites;
    
    for(uint32_t id : diff_result)
    {
        //m_sprite_buffers.erase(id);
        m_shadow_data_cache.erase(id);
        m_shadow_buffers.erase(id);
    }
}

math::Quad SpriteBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
