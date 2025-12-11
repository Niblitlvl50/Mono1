
#include "SpriteBufferFactory.h"

#include "Rendering/Sprite/ISprite.h"
#include "Rendering/RenderBuffer/BufferFactory.h"

#include "Math/MathFunctions.h"

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
