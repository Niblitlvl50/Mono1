
#include "SpriteBatchDrawer.h"

#include "ISprite.h"
#include "SpriteSystem.h"
#include "SpriteProperties.h"
#include "Rendering/IRenderer.h"
#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Rendering/Shader/SpriteShader.h"
#include "Rendering/Sprite/ISpriteFactory.h"
#include "TransformSystem/TransformSystem.h"

#include "Math/Quad.h"

#include <utility>

using namespace mono;

namespace
{
    struct SpriteTransformPair
    {
        math::Matrix transform;
        math::Quad world_bb;
        mono::ISprite* sprite;
        int layer;
    };
}

SpriteBatchDrawer::SpriteBatchDrawer(const mono::TransformSystem* transform_system, mono::SpriteSystem* sprite_system)
    : m_transform_system(transform_system)
    , m_sprite_system(sprite_system)
{ }

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

        const math::Vector& uv_upper_left = frame.uv_upper_left;
        const math::Vector& uv_lower_right = frame.uv_lower_right;
        uv_coordinates.emplace_back(uv_upper_left.x, uv_upper_left.y);
        uv_coordinates.emplace_back(uv_upper_left.x, uv_lower_right.y);
        uv_coordinates.emplace_back(uv_lower_right.x, uv_lower_right.y);
        uv_coordinates.emplace_back(uv_lower_right.x, uv_upper_left.y);

        height_values.push_back(0.0f);
        height_values.push_back(frame.size.y);
        height_values.push_back(frame.size.y);
        height_values.push_back(0.0f);
    }

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

    SpriteDrawBuffers buffers;
    buffers.vertices_per_sprite = 4;
    buffers.vertices = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, vertices.size() * 2);
    buffers.offsets = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, vertex_offsets.size() * 2);
    buffers.uv = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, uv_coordinates.size() * 2);
    buffers.heights = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 1, height_values.size());

    buffers.vertices->UpdateData(vertices.data(), 0, vertices.size() * 2);
    buffers.offsets->UpdateData(vertex_offsets.data(), 0, vertex_offsets.size() * 2);
    buffers.uv->UpdateData(uv_coordinates.data(), 0, uv_coordinates.size() * 2);
    buffers.heights->UpdateData(height_values.data(), 0, height_values.size());

    mono::ClearRenderBuffer();

    m_sprite_buffers[sprite_data->hash] = std::move(buffers);
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
            
            const math::Matrix& transform = m_transform_system->GetWorld(id);
            sprites_to_draw.push_back({ transform, world_bounds, sprite, layer });
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

        const uint32_t sprite_properties = sprite_transform.sprite->GetProperties();
        if(sprite_properties & mono::SpriteProperty::SHADOW)
        {
            const math::Vector shadow_offset = sprite_transform.sprite->GetShadowOffset();
            const float shadow_size = sprite_transform.sprite->GetShadowSize();
            constexpr mono::Color::RGBA shadow_color(0.2f, 0.2f, 0.2f, 0.7f);
            renderer.DrawFilledCircle(shadow_offset, math::Vector(shadow_size, shadow_size / 2.0f), 8, shadow_color);
        }

        //renderer.DrawSprite(*sprite_transform.sprite);

        mono::IShader* sprite_shader = renderer.GetSpriteShader();
        renderer.UseShader(sprite_shader);

        const bool flip_horizontal = (sprite_transform.sprite->GetHorizontalDirection() == mono::HorizontalDirection::LEFT);
        const bool flip_vertical = (sprite_transform.sprite->GetVerticalDirection() == mono::VerticalDirection::DOWN);

        SpriteShader::SetShade(sprite_shader, sprite_transform.sprite->GetShade());
        SpriteShader::FlipHorizontal(sprite_shader, flip_horizontal);
        SpriteShader::FlipVertical(sprite_shader, flip_vertical);
        SpriteShader::SetWindSway(sprite_shader, sprite_properties & mono::SpriteProperty::WIND_SWAY);

        const SpriteDrawBuffers& sprite_buffers = m_sprite_buffers[sprite_transform.sprite->GetSpriteHash()];
        const int offset = sprite_transform.sprite->GetCurrentFrameIndex() * sprite_buffers.vertices_per_sprite;
        mono::ITexture* texture = sprite_transform.sprite->GetTexture();

        renderer.DrawSprite(
            sprite_buffers.vertices.get(),
            sprite_buffers.uv.get(),
            sprite_buffers.heights.get(),
            offset,
            texture);
    }
}

math::Quad SpriteBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
