
#pragma once

#include "System/SysTypes.h"

namespace mono
{
    class IRenderBuffer
    {
    public:

        virtual ~IRenderBuffer()
        { }

        virtual void UpdateData(const void* data, uint offset, uint count) = 0;
        virtual void Use() const = 0;
        virtual uint Id() const = 0;
    };

    void ClearRenderBuffer();
}
