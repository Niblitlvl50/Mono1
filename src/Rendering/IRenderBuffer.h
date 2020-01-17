
#pragma once

#include <cstdlib>

namespace mono
{
    class IRenderBuffer
    {
    public:

        virtual ~IRenderBuffer() = default;
        virtual void UpdateData(const void* data, size_t offset, size_t count) = 0;
        virtual void Use() const = 0;
        virtual uint32_t Id() const = 0;
    };

    void ClearRenderBuffer();
}
