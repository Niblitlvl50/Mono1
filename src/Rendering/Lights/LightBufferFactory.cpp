
#include "LightBufferFactory.h"
#include "Rendering/Color.h"
#include "Rendering/RenderBuffer/BufferFactory.h"
#include "Math/Vector.h"

mono::LightDrawBuffers mono::AllocateLightDrawBuffers(uint32_t num_lights)
{
    mono::LightDrawBuffers draw_buffers;
    draw_buffers.vertices   = CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 2, num_lights * 4, nullptr, "light_draw_buffer");
    draw_buffers.uv         = CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 2, num_lights * 4, nullptr, "light_draw_buffer");
    draw_buffers.colors     = CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 4, num_lights * 4, nullptr, "light_draw_buffer");
    draw_buffers.indices    = CreateElementBuffer(BufferType::DYNAMIC, num_lights * 6, nullptr);

    return draw_buffers;
}

void mono::UpdateLightDrawBuffer(
    const std::vector<math::Vector>& vertex_data,
    const std::vector<math::Vector>& uv_data,
    const std::vector<mono::Color::RGBA>& color_data,
    const std::vector<uint16_t>& index_data,
    mono::LightDrawBuffers& draw_buffers)
{
    draw_buffers.vertices->UpdateData(vertex_data.data(), 0, vertex_data.size());
    draw_buffers.uv->UpdateData(uv_data.data(), 0, uv_data.size());
    draw_buffers.colors->UpdateData(color_data.data(), 0, color_data.size());
    draw_buffers.indices->UpdateData(index_data.data(), 0, index_data.size());
}
