
#pragma once

#include "IRenderBuffer.h"
#include <memory>

namespace mono
{
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

    std::unique_ptr<IRenderBuffer> CreateRenderBuffer(mono::BufferType type, mono::BufferData data, int components, size_t count);
    std::unique_ptr<IElementBuffer> CreateElementBuffer(mono::BufferType type, mono::BufferData data, size_t count);
}
