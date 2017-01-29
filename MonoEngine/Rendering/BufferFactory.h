
#pragma once

#include "IRenderBuffer.h"
#include <memory>

namespace mono
{
    std::unique_ptr<IRenderBuffer> CreateRenderBuffer(mono::BufferType type, mono::BufferData data);
}
