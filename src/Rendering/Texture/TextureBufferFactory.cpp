
#include "TextureBufferFactory.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Rendering/Texture/ITexture.h"
#include "Math/Vector.h"

mono::TextureDrawBuffers mono::BuildTextureDrawBuffers(const mono::ITexture* texture)
{
    const float half_width = texture->Width() / 2.0f;
    const float half_height = texture->Height() / 2.0f;

    const math::Vector vertex_data[] = {
        { -half_width, -half_height },
        { -half_width, +half_height },
        { +half_width, +half_height },
        { +half_width, -half_height }
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
    draw_buffers.vertices = mono::CreateRenderBuffer(mono::BufferType::STATIC, mono::BufferData::FLOAT, 2, 4, vertex_data);
    draw_buffers.uv = mono::CreateRenderBuffer(mono::BufferType::STATIC, mono::BufferData::FLOAT, 2, 4, uv_data);
    draw_buffers.indices = mono::CreateElementBuffer(mono::BufferType::STATIC, 6, index_data);

    return draw_buffers;
}
