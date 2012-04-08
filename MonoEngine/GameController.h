//
//  GameController.h
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//


#ifndef MONO_GAMECONTROLLER_H
#define MONO_GAMECONTROLLER_H

#include "MonoFwd.h"
#include <stack>

namespace mono
{
    class GameController
    {
    public:
        static GameController& Instance();
        
        void InsertZone(IZonePtr zone);
        IZonePtr GetCurrentZone();
        
    private:
        GameController();
        
        std::stack<IZonePtr> mZoneStack;
    };
}

#endif

