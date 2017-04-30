
#pragma once

#include <memory>

using byte = unsigned char;

namespace System
{
    struct IImage
    {
        virtual ~IImage()
        { }

        //! Get the raw image data
        //! \return byte* Pointer to the data
        virtual const byte* Data() const = 0;

        //! Return the width
        virtual unsigned int Width() const = 0;

        //! Return the height
        virtual unsigned int Height() const = 0;

        //! Number of color components in the data. 1 = alpha, 3 = rgb, 4 = rgba
        virtual int ColorComponents() const = 0;
    };
    
    using IImagePtr = std::shared_ptr<IImage>;
}

