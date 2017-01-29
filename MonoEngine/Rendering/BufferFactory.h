
#pragma once

#include "IRenderBuffer.h"
#include <memory>

namespace mono
{
    enum class BufferType
    {
        STATIC,
        DYNAMIC
    };

    enum class BufferData
    {
        INT,
        FLOAT
    };

    std::unique_ptr<IRenderBuffer> CreateRenderBuffer(mono::BufferType type, mono::BufferData data, uint count);
}
