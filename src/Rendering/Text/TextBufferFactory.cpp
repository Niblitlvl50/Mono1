
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
