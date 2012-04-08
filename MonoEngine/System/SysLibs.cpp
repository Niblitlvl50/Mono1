//
//  SysLibs.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-12.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "SysLibs.h"
#include "SDL.h"
#include <stdexcept>

void Libs::Init()
{
	// Init SDL video subsystem
	const int result = SDL_Init(SDL_INIT_VIDEO);
    if(result != 0)
        throw std::runtime_error("Couldn't initialize SDL");
}

void Libs::Exit()
{
	SDL_Quit();
}
