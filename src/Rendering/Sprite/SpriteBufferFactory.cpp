
#include "SpriteBufferFactory.h"

#include "Rendering/Color.h"
#include "Rendering/Sprite/ISprite.h"
#include "Rendering/RenderBuffer/BufferFactory.h"

#include "Math/MathFunctions.h"

mono::SpriteDrawBuffers mono::BuildSpriteDrawBuffers(const mono::SpriteData* sprite_data)
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

    return buffers;
}

mono::SpriteShadowBuffers mono::BuildSpriteShadowBuffers(const math::Vector& shadow_offset, float shadow_size)
{
    const math::Vector size(shadow_size, shadow_size / 2.0f);

    const int segments = 8;
    std::vector<math::Vector> vertices;
    vertices.reserve(segments +1);
    vertices.push_back(shadow_offset);

    const uint16_t n_indices = segments * 3;
    std::vector<uint16_t> indices;
    indices.reserve(n_indices);

    const float coef = 2.0f * math::PI() / float(segments);

    for(int index = 0; index < segments; ++index)
    {
        const float radians = index * coef;
        const float x = size.x * std::cos(radians) + shadow_offset.x;
        const float y = size.y * std::sin(radians) + shadow_offset.y;
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

    return buffers;
}
