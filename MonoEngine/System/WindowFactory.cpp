//
//  WindowFactory.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-12.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "WindowFactory.h"
#include "SDL/SDLWindow.h"

mono::IWindowPtr mono::CreateWindow(const char* title, int width, int height, bool fullscreen)
{
    return std::make_shared<SDLWindow>(title, width, height, fullscreen);
}
