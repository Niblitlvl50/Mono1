//
//  WindowFactory.h
//  Monolith1
//
//  Created by Niblit on 2011-12-12.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#pragma once

#include "MonoPtrFwd.h"

namespace mono
{
    //! Creates a opengl window
    IWindowPtr CreateWindow(const std::string& title, int width, int height, bool fullscreen);
}

