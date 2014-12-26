//
//  TextureFactory.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoPtrFwd.h"
#include "SysTypes.h"

namespace mono
{
    //! Create a texture from disk, only PNG is supported at the moment
    ITexturePtr CreateTexture(const std::string& source);

    //! Create a texture from memory
    ITexturePtr CreateTexture(const byte* data, int width, int height, int colorComponents, unsigned int targetFormat);
}
