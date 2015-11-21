//
//  SysLibs.cpp
//  Monolith1
//
//  Created by Niblit on 2011-12-12.
//  Copyright 2011 __MyCompanyName__. All rights reserved.
//

#include "SysLibs.h"
#include "TextFunctions.h"
#include "ShaderFunctions.h"
#include "SDL.h"
#include <stdexcept>
#include <cstdio>

void System::Init()
{
    // Init SDL video subsystem
    const int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    if(result != 0)
    {
        const std::string error = "Couldn't initialize SDL" + std::string(SDL_GetError());
        throw std::runtime_error(error);
    }

    SDL_version version;
    SDL_GetVersion(&version);

    std::printf("Initiated libSDL, version: %d.%d.%d\n", version.major, version.minor, version.patch);

    mono::LoadDefaultShaderFactory();
}

void System::Exit()
{
    mono::UnloadShaderFactory();
    mono::UnloadFont();

    SDL_Quit();
}
