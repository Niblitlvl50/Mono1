//
//  WindowFactory.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-12.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "WindowFactory.h"
#include "SDLWindow.h"
#include "SDL_video.h"


mono::IWindowPtr mono::CreateOpenGLWindow(const std::string& title, int width, int height, bool fullscreen, IOGLSetupPtr setup)
{
    const unsigned int screenflag = fullscreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;
	const unsigned int flags = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | screenflag;
    
    IWindowPtr window(new SDLWindow(title, width, height, flags, setup));
    return window;
}
