//
//  SysOpenGL.h
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once


#include "SDL.h"

//#include "SDL_opengles.h"


#include <SDL_opengl.h>


namespace OGL
{
    struct OGLPushPopMatrix
    {
        OGLPushPopMatrix();
        ~OGLPushPopMatrix();
    };
}
