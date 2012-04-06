//
//  OpenGLSetup.h
//  Monolith1
//
//  Created by Niblit on 2012-04-03.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef GAME_OPENGLSETUP_H
#define GAME_OPENGLSETUP_H

#include "IOGLSetup.h"

namespace game
{
    struct OpenGLSetup : public mono::IOGLSetup
    {
        virtual void OnCreated();
        virtual void OnChanged(int width, int height);        
    };
}

#endif

