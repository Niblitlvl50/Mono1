//
//  SysOpenGL.h
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#ifdef __IPHONEOS__
//#include "SDL_opengles.h"
#include "SDL_opengles2.h"
#else
#include "SDL_opengl.h"
#endif
