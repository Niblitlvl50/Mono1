
#include "BufferFactory.h"
#include "RenderBuffer.h"

std::unique_ptr<mono::IRenderBuffer> mono::CreateRenderBuffer(BufferType type, BufferData data, size_t count)
{
    return std::make_unique<mono::RenderBuffer>(type, data, count);
}
