
#pragma once

#include "IImage.h"

namespace System
{
    //! Load an image from disk, right now only PNG is supported
    //! \param source Where the image is located
    //! \return A smart image pointer
    IImagePtr LoadImage(const char* source);

    IImagePtr LoadImageFromData(const unsigned char* data, int data_length);

    //! Create an image from memory
    //!
    //! \param data The image data
    //! \param width Image width
    //! \param height Image height
    //! \param colorComponents 
    //!
    //! \return A smart image pointer
    IImagePtr CreateImage(const byte* data, int width, int height, int colorComponents);
}
