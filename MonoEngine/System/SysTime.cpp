//
//  Time.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-12.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "SysTime.h"
//#include <SDL_timer.h>

#include "SDL.h"

unsigned int Time::GetMilliseconds()
{
    return SDL_GetTicks();
}

void Time::Sleep(unsigned int milliseconds)
{
    SDL_Delay(milliseconds);
}
