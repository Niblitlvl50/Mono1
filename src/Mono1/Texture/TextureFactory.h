//
//  TextureFactory.h
//  Monolith1
//
//  Created by Niblit on 2011-12-10.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#ifndef MONO_TEXTUREFACTORY_H
#define MONO_TEXTUREFACTORY_H

#include "MonoFwd.h"

namespace mono
{
    ITexturePtr CreateTexture(const std::string& source);
}

#endif

