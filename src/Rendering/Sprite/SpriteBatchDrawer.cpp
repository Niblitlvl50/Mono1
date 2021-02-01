
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

        BuildSpriteDrawBuffers(sprite_data);
    }
}

void SpriteBatchDrawer::BuildSpriteDrawBuffers(const mono::SpriteData* sprite_data) const
{
    std::vector<math::Vector> vertices;
    std::vector<math::Vector> vertex_offsets;
    std::vector<math::Vector> uv_coordinates;
    std::vector<math::Vector> uv_coordinates_flipped;
    std::vector<float> height_values;

    for(const mono::SpriteFrame& frame : sprite_data->frames)
    {
        const math::Vector& sprite_half_size = frame.size / 2.0f;

        vertices.emplace_back(-sprite_half_size.x, -sprite_half_size.y);
        vertices.emplace_back(-sprite_half_size.x,  sprite_half_size.y);
        vertices.emplace_back( sprite_half_size.x,  sprite_half_size.y);
        vertices.emplace_back( sprite_half_size.x, -sprite_half_size.y);

        vertex_offsets.emplace_back(frame.center_offset);
        vertex_offsets.emplace_back(frame.center_offset);
        vertex_offsets.emplace_back(frame.center_offset);
        vertex_offsets.emplace_back(frame.center_offset);

        math::Vector uv_upper_left = frame.uv_upper_left;
        math::Vector uv_lower_right = frame.uv_lower_right;
        uv_coordinates.emplace_back(uv_upper_left.x, uv_upper_left.y);
        uv_coordinates.emplace_back(uv_upper_left.x, uv_lower_right.y);
        uv_coordinates.emplace_back(uv_lower_right.x, uv_lower_right.y);
        uv_coordinates.emplace_back(uv_lower_right.x, uv_upper_left.y);

        std::swap(uv_upper_left.x, uv_lower_right.x);
        std::swap(uv_lower_right.y, uv_upper_left.y);
        uv_coordinates_flipped.emplace_back(uv_upper_left.x, uv_upper_left.y);
        uv_coordinates_flipped.emplace_back(uv_upper_left.x, uv_lower_right.y);
        uv_coordinates_flipped.emplace_back(uv_lower_right.x, uv_lower_right.y);
        uv_coordinates_flipped.emplace_back(uv_lower_right.x, uv_upper_left.y);

        height_values.push_back(0.0f);
        height_values.push_back(frame.size.y);
        height_values.push_back(frame.size.y);
        height_values.push_back(0.0f);
    }

/*
    // Add flipped uv coordinates at the end...
    for(const mono::SpriteFrame& frame : sprite_data->frames)
    {
        math::Vector uv_upper_left = frame.uv_upper_left;
        math::Vector uv_lower_right = frame.uv_lower_right;

        std::swap(uv_upper_left.x, uv_lower_right.x);
        std::swap(uv_lower_right.y, uv_upper_left.y);

        uv_coordinates.emplace_back(uv_upper_left.x, uv_upper_left.y);
        uv_coordinates.emplace_back(uv_upper_left.x, uv_lower_right.y);
        uv_coordinates.emplace_back(uv_lower_right.x, uv_lower_right.y);
        uv_coordinates.emplace_back(uv_lower_right.x, uv_upper_left.y);
    }
    */

    SpriteDrawBuffers buffers;
    buffers.vertices_per_sprite = 4;
    buffers.vertices = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, vertices.size(), vertices.data());
    buffers.offsets = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, vertex_offsets.size(), vertex_offsets.data());
    buffers.uv = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, uv_coordinates.size(), uv_coordinates.data());
    buffers.uv_flipped = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, uv_coordinates_flipped.size(), uv_coordinates_flipped.data());
    buffers.heights = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 1, height_values.size(), height_values.data());

    m_sprite_buffers[sprite_data->hash] = std::move(buffers);
}

void SpriteBatchDrawer::BuildSpriteShadowBuffers(const ISprite* sprite, uint32_t id) const
{
    const math::Vector& offset = sprite->GetShadowOffset();
    const float shadow_size = sprite->GetShadowSize();
    const math::Vector size(shadow_size, shadow_size / 2.0f);

    const int segments = 8;
    std::vector<math::Vector> vertices;
    vertices.reserve(segments +1);
    vertices.push_back(offset);

    const uint16_t n_indices = segments * 3;
    std::vector<uint16_t> indices;
    indices.reserve(n_indices);

    const float coef = 2.0f * math::PI() / float(segments);

    for(int index = 0; index < segments; ++index)
    {
        const float radians = index * coef;
        const float x = size.x * std::cos(radians) + offset.x;
        const float y = size.y * std::sin(radians) + offset.y;
        vertices.emplace_back(x, y);

        indices.push_back(0);
        indices.push_back(index +1);
        indices.push_back(index +2);
    }

    indices.pop_back();
    indices.pop_back();

    indices.push_back(vertices.size() -1);
    indices.push_back(1);

    constexpr mono::Color::RGBA shadow_color(0.2f, 0.2f, 0.2f, 0.5f);
    const std::vector<mono::Color::RGBA> colors(vertices.size(), shadow_color);

    SpriteShadowBuffers buffers;
    buffers.vertices = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, vertices.size(), vertices.data());
    buffers.colors = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 4, colors.size(), colors.data());
    buffers.indices = mono::CreateElementBuffer(BufferType::STATIC, indices.size(), indices.data());
    m_shadow_buffers[id] = std::move(buffers);
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
                BuildSpriteDrawBuffers(sprite->GetSpriteData());

            const bool has_shadow = (sprite->GetProperties() & mono::SpriteProperty::SHADOW);
            if(has_shadow)
            {
                auto shadow_it = m_shadow_buffers.find(id);
                if(shadow_it == m_shadow_buffers.end())
                    BuildSpriteShadowBuffers(sprite, id);
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

        const auto it = m_shadow_buffers.find(sprite_transform.entity_id);
        if(it != m_shadow_buffers.end())
        {
            const SpriteShadowBuffers& shadow_buffers = it->second;
            renderer.DrawTrianges(
                shadow_buffers.vertices.get(),
                shadow_buffers.colors.get(),
                shadow_buffers.indices.get(),
                shadow_buffers.indices->Size());
        }

        const SpriteDrawBuffers& sprite_buffers = m_sprite_buffers[sprite_transform.sprite->GetSpriteHash()];
        const int offset = sprite_transform.sprite->GetCurrentFrameIndex() * sprite_buffers.vertices_per_sprite;
        mono::ITexture* texture = sprite_transform.sprite->GetTexture();

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
