//
//  SysVideo.cpp
//  MonoiOS
//
//  Created by Niblit on 2012-09-17.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SysVideo.h"
#include "SDL.h"
#include "Vector2f.h"

math::Vector2f Video::GetCurrentWindowSize()
{
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    
    return math::Vector2f(mode.w, mode.h);
}
