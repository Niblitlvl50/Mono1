//
//  SysVideo.cpp
//  MonoiOS
//
//  Created by Niblit on 2012-09-17.
//  Copyright (c) 2012 __MyCompanyName__. All rights reserved.
//

#include "SysVideo.h"
#include "Math/Vector2f.h"
#include "SDL_video.h"

math::Vector Video::GetCurrentWindowSize()
{
    SDL_DisplayMode mode;
    SDL_GetCurrentDisplayMode(0, &mode);
    
    return math::Vector(mode.w, mode.h);
}
