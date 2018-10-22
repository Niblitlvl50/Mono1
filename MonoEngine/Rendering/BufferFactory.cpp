
#include "BufferFactory.h"
#include "RenderBuffer.h"

std::unique_ptr<mono::IRenderBuffer> mono::CreateRenderBuffer(
    mono::BufferTarget target, mono::BufferType type, mono::BufferData data, size_t count)
{
    return std::make_unique<mono::RenderBuffer>(target, type, data, count);
}
