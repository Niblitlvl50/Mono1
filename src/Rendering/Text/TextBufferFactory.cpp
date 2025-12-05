
#include "TextBufferFactory.h"
#include "TextFunctions.h"
#include "Rendering/RenderBuffer/BufferFactory.h"

mono::TextDrawBuffers mono::BuildTextDrawBuffers(int font_id, const char* text, mono::FontCentering center_flags)
{
    const mono::TextDefinition& def = mono::GenerateVertexDataFromString(font_id, text, center_flags);

    TextDrawBuffers buffers;
    buffers.vertices = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, def.vertices.size(), def.vertices.data());
    buffers.uv = CreateRenderBuffer(BufferType::STATIC, BufferData::FLOAT, 2, def.uv_coords.size(), def.uv_coords.data());
    buffers.indices = CreateElementBuffer(BufferType::STATIC, def.indices.size(), def.indices.data());

    return buffers;
}

mono::TextDrawBuffers mono::AllocateTextDrawBuffers(uint32_t size)
{
    TextDrawBuffers buffers;
    buffers.vertices = CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 2, size * 4, nullptr);
    buffers.uv = CreateRenderBuffer(BufferType::DYNAMIC, BufferData::FLOAT, 2, size * 4, nullptr);
    buffers.indices = CreateElementBuffer(BufferType::DYNAMIC, size * 6, nullptr);

    return buffers;
}

void mono::UpdateTextBuffer(int font_id, const char* text, mono::FontCentering center_flags, TextDrawBuffers& draw_buffers)
{
    const mono::TextDefinition& def = mono::GenerateVertexDataFromString(font_id, text, center_flags);

    draw_buffers.vertices->UpdateData(def.vertices.data(), 0, def.vertices.size());
    draw_buffers.uv->UpdateData(def.uv_coords.data(), 0, def.uv_coords.size());
    draw_buffers.indices->UpdateData(def.indices.data(), 0, def.indices.size());
}
