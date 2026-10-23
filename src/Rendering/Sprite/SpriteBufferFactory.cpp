
#include "SpriteBufferFactory.h"

#include "Rendering/Sprite/ISprite.h"
#include "Rendering/RenderBuffer/BufferFactory.h"

#include "Math/MathFunctions.h"

namespace
{
    // Each sprite frame is a stack of strips rather than a single quad, so the wind sway can bend
    // the sprite from some way up instead of only leaning the whole of it.
    constexpr int SPRITE_STRIPS = 4;
    constexpr int SPRITE_ROWS = SPRITE_STRIPS + 1;
    constexpr int VERTICES_PER_SPRITE = SPRITE_ROWS * 2;
}

mono::SpriteDrawBuffers mono::BuildSpriteDrawBuffers(const mono::SpriteData* sprite_data, const char* debug_context)
{
    std::vector<math::Vector> vertices;
    std::vector<math::Vector> vertex_offsets;
    std::vector<math::Vector> uv_coordinates;
    std::vector<math::Vector> uv_coordinates_flipped;
    std::vector<float> height_values;

    for(const mono::SpriteFrame& frame : sprite_data->frames)
    {
        const math::Vector& sprite_half_size = frame.size / 2.0f;

        // The bottom of the sprite is at the upper left uv's y, as with the quad this replaces.
        const math::Vector uv_upper_left = frame.uv_upper_left;
        const math::Vector uv_lower_right = frame.uv_lower_right;

        // Rows go from the bottom to the top, a left and a right vertex in each.
        for(int row = 0; row < SPRITE_ROWS; ++row)
        {
            const float t = float(row) / float(SPRITE_STRIPS);
            const float y = -sprite_half_size.y + (frame.size.y * t);
            const float uv_y = uv_upper_left.y + (uv_lower_right.y - uv_upper_left.y) * t;
            const float uv_y_flipped = uv_lower_right.y + (uv_upper_left.y - uv_lower_right.y) * t;

            vertices.emplace_back(-sprite_half_size.x, y);
            vertices.emplace_back( sprite_half_size.x, y);

            vertex_offsets.emplace_back(frame.center_offset);
            vertex_offsets.emplace_back(frame.center_offset);

            uv_coordinates.emplace_back(uv_upper_left.x, uv_y);
            uv_coordinates.emplace_back(uv_lower_right.x, uv_y);

            uv_coordinates_flipped.emplace_back(uv_lower_right.x, uv_y_flipped);
            uv_coordinates_flipped.emplace_back(uv_upper_left.x, uv_y_flipped);

            // Distance from the bottom of the sprite.
            height_values.push_back(frame.size.y * t);
            height_values.push_back(frame.size.y * t);
        }
    }

    std::vector<uint16_t> indices;
    for(int strip = 0; strip < SPRITE_STRIPS; ++strip)
    {
        const uint16_t bottom_left = uint16_t(strip * 2);
        const uint16_t bottom_right = uint16_t(bottom_left + 1);
        const uint16_t top_left = uint16_t(bottom_left + 2);
        const uint16_t top_right = uint16_t(bottom_left + 3);

        indices.insert(indices.end(), { bottom_left, top_left, top_right, bottom_left, top_right, bottom_right });
    }

    SpriteDrawBuffers buffers;
    buffers.vertices_per_sprite = VERTICES_PER_SPRITE;
    buffers.vertices =
        mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, vertices.size(), vertices.data(), debug_context);
    buffers.offsets =
        mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, vertex_offsets.size(), vertex_offsets.data(), debug_context);
    buffers.uv =
        mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, uv_coordinates.size(), uv_coordinates.data(), debug_context);
    buffers.uv_flipped =
        mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, uv_coordinates_flipped.size(), uv_coordinates_flipped.data(), debug_context);
    buffers.heights =
        mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 1, height_values.size(), height_values.data(), debug_context);
    buffers.indices =
        mono::CreateElementBuffer(BufferType::STATIC, indices.size(), indices.data(), debug_context);

    return buffers;
}

mono::SpriteShadowBuffers mono::BuildSpriteShadowBuffers(const math::Vector& shadow_offset, float shadow_radius)
{
    const math::Vector size = { shadow_radius, shadow_radius / 2.0f };

    const math::Vector vertices[] = {
        math::Vector(-size.x, -size.y) + shadow_offset,
        math::Vector(-size.x,  size.y) + shadow_offset,
        math::Vector( size.x,  size.y) + shadow_offset,
        math::Vector( size.x, -size.y) + shadow_offset,
    };

    const math::Vector uvs[] = {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
        { 1.0f, 0.0f },
    };

    SpriteShadowBuffers buffers;
    buffers.vertices = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, std::size(vertices), vertices, "sprite_shadow_buffer");
    buffers.uv = mono::CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, std::size(uvs), uvs, "sprite_shadow_buffer");

    return buffers;
}
