//
//  ImageFactory.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "ImageFactory.h"
#include <stdexcept>

#include "PNGImage.h"

mono::IImagePtr mono::LoadImage(const std::string& source)
{
    const size_t dotpos = source.find_last_of(".");
    if(dotpos == std::string::npos)
        throw std::runtime_error("Unable to determine extension");
    
    const std::string extension = source.substr(dotpos +1);
    
    IImagePtr image;
    
    if(extension.compare("png") == 0)
        image.reset(new libpng::PNGImage(source));
    else
        throw std::runtime_error("Unsupported image");
    
    return image;
}
