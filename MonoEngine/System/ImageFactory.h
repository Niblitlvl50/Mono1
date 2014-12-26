//
//  ImageFactory.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IImage.h"
#include <string>

namespace mono
{
    //! Load an image from disk, right now only PNG is supported
    //! \param source Where the image is located
    //! \return A smart image pointer
    IImagePtr LoadImage(const std::string& source);

    //! Create an image from memory
    //!
    //! \param data The image data
    //! \param width Image width
    //! \param height Image height
    //! \param colorComponents ...
    //! \param targetFormat ...
    //!
    //! \return A smart image pointer
    IImagePtr CreateImage(const byte* data, int width, int height, int colorComponents, unsigned int targetFormat);
}
