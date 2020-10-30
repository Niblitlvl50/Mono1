
#pragma once

#include "IRenderBuffer.h"
#include <memory>

namespace mono
{
    enum class BufferTarget : int
    {
        ARRAY_BUFFER,
        ELEMENT_BUFFER
    };

    enum class BufferType : int
    {
        STATIC,
        DYNAMIC
    };

    enum class BufferData
    {
        INT,
        FLOAT
    };

    std::unique_ptr<IRenderBuffer> CreateRenderBuffer(
        mono::BufferTarget target, mono::BufferType type, mono::BufferData data, size_t count);
}
