
#pragma once

#include <cstddef>

namespace mono
{
    class IRenderBuffer
    {
    public:

        virtual ~IRenderBuffer()
        { }

        virtual void UpdateData(const void* data, size_t offset, size_t count) = 0;
        virtual void Use() const = 0;
        virtual size_t Id() const = 0;
    };

    void ClearRenderBuffer();
}
