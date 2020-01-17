
#pragma once

#include <cstdint>

namespace mono
{
    class ITexture
    {
    public:

        virtual ~ITexture() = default;

        //! Use the texture
        virtual void Use() const = 0;

        //! Get the internal texture id
        virtual uint32_t Id() const = 0;
        virtual uint32_t Width() const = 0;
        virtual uint32_t Height() const = 0;
    };

    void ClearTexture();
}

