
#pragma once

#include "System/SysTypes.h"

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

    class IRenderBuffer
    {
    public:

        virtual ~IRenderBuffer()
        { }

        virtual void Initialize(uint count) = 0;
        virtual void UpdateData(const void* data, uint offset, uint count) = 0;
        virtual void Use() const = 0;
        virtual uint Id() const = 0;
    };

    void ClearRenderBuffer();
}
