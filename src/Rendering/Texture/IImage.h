
#pragma once

#include <memory>
#include <cstdint>

using byte = unsigned char;

namespace mono
{
    struct IImage
    {
        virtual ~IImage() = default;

        //! Get the raw image data
        virtual const byte* Data() const = 0;
        virtual uint32_t Width() const = 0;
        virtual uint32_t Height() const = 0;

        //! Number of color components in the data. 1 = alpha, 3 = rgb, 4 = rgba
        virtual int ColorComponents() const = 0;
    };
    
    using IImagePtr = std::shared_ptr<IImage>;
}

