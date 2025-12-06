
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

    enum class BufferData : int
    {
        INT,
        INT_16,
        FLOAT,
    };

    std::unique_ptr<IRenderBuffer> CreateRenderBuffer(
        BufferType type,
        BufferData data,
        uint32_t components,
        uint32_t count,
        const void* data_ptr,
        const char* debug_label);
    
    std::unique_ptr<IElementBuffer> CreateElementBuffer(
        BufferType type,
        uint32_t count,
        const uint16_t* data_ptr,
        const char* debug_label);
}
