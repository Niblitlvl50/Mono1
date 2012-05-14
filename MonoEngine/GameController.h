//
//  GameController.h
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MonoFwd.h"

namespace mono
{
    struct IGameController
    {
        virtual ~IGameController()
        { }
    };
    
    IGameController& GameControllerInstance();
}

