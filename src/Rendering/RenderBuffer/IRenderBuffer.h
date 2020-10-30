
#pragma once

#include <cstdint>

namespace mono
{
    class IRenderBuffer
    {
    public:

        virtual ~IRenderBuffer() = default;
        virtual void UpdateData(const void* data, uint32_t offset, uint32_t count) = 0;
        virtual void Use() const = 0;
        virtual uint32_t Size() const = 0;
        virtual uint32_t Id() const = 0;
    };

    void ClearRenderBuffer();
}
