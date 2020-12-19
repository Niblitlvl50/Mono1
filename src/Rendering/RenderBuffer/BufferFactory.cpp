
#include "BufferFactory.h"
#include "RenderBuffer.h"

std::unique_ptr<mono::IRenderBuffer> mono::CreateRenderBuffer(
    mono::BufferType type, mono::BufferData data, int components, size_t count)
{
    return std::make_unique<mono::RenderBuffer>(type, data, components, count);
}

std::unique_ptr<mono::IElementBuffer> mono::CreateElementBuffer(
    mono::BufferType type, mono::BufferData data, size_t count)
{
    return std::make_unique<mono::ElementBuffer>(type, data, count);
}
