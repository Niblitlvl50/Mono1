//
//  WindowFactory.h
//  Monolith1
//
//  Created by Niblit on 2011-12-12.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//


#ifndef SYSTEM_WINDOW_FACTORY_H
#define SYSTEM_WINDOW_FACTORY_H

#include "MonoFwd.h"

namespace mono
{
    IWindowPtr CreateOpenGLWindow(int width, int height, bool fullscreen, IOGLSetupPtr setup);
}

#endif

