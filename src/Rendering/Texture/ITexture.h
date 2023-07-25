
#pragma once

#include <cstdint>

namespace mono
{
    class ITexture
    {
    public:

        virtual ~ITexture() = default;

        //! Get the internal texture id
        virtual uint32_t Id() const = 0;
        virtual uint32_t Width() const = 0;
        virtual uint32_t Height() const = 0;
        virtual bool IsAlphaTexture() const = 0;
    };

    class ISampler
    {
    public:

        virtual ~ISampler() = default;
        virtual uint32_t Id() const = 0;
    };
}

