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
    IImagePtr LoadImage(const std::string& source);
}
