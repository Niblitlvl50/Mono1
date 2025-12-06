
#include "TextureBufferFactory.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Rendering/Texture/ITexture.h"
#include "Math/Vector.h"

mono::TextureDrawBuffers mono::BuildTextureDrawBuffers(const mono::ITexture* texture, const mono::TextureBufferOptions& options)
{
    const float half_width = (texture->Width() / 2.0f) / options.pixels_per_meter;
    const float half_height = (texture->Height() / 2.0f) / options.pixels_per_meter;
    
    const math::Vector shift_vector = mono::CalculateOffsetFromCenter(options.anchor, math::Vector(texture->Width(), texture->Height()), options.pixels_per_meter);

    const math::Vector vertex_data[] = {
        math::Vector(-half_width, -half_height) + shift_vector,
        math::Vector(-half_width, +half_height) + shift_vector,
        math::Vector(+half_width, +half_height) + shift_vector,
        math::Vector(+half_width, -half_height) + shift_vector,
    };

    const math::Vector uv_data[] = {
        { 0.0f, 1.0f },
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f }
    };

    const uint16_t index_data[] = {
        0, 1, 2,
        0, 2, 3
    };

    mono::TextureDrawBuffers draw_buffers;
    draw_buffers.vertices = mono::CreateRenderBuffer(mono::BufferType::STATIC, mono::BufferData::FLOAT, 2, 4, vertex_data, "texture_draw_buffer");
    draw_buffers.uv = mono::CreateRenderBuffer(mono::BufferType::STATIC, mono::BufferData::FLOAT, 2, 4, uv_data, "texture_draw_buffer");
    draw_buffers.indices = mono::CreateElementBuffer(mono::BufferType::STATIC, 6, index_data, "texture_draw_buffer");

    return draw_buffers;
}

math::Vector mono::CalculateOffsetFromCenter(mono::AnchorPoint anchor_point, const math::Vector& texture_size, float pixels_per_meter)
{
    const float half_width = (texture_size.x / 2.0f) / pixels_per_meter;
    const float half_height = (texture_size.y / 2.0f) / pixels_per_meter;

    math::Vector shift_vector = math::ZeroVec;

    switch(anchor_point)
    {
    case mono::AnchorPoint::CENTER:
        break;
    case mono::AnchorPoint::TOP_LEFT:
        shift_vector = math::Vector(half_width, -half_height);
        break;
    case mono::AnchorPoint::BOTTOM_LEFT:
        shift_vector = math::Vector(half_width, half_height);
        break;
    case mono::AnchorPoint::TOP_RIGHT:
        shift_vector = math::Vector(-half_width, -half_height);
        break;
    case mono::AnchorPoint::BOTTOM_RIGHT:
        shift_vector = math::Vector(-half_width, half_height);
        break;
    }

    return shift_vector;
}
