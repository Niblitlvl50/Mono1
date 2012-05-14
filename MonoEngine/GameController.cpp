//
//  GameController.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "GameController.h"
#include "IZone.h"
#include "Vector2f.h"

namespace
{
    class GameController : public mono::IGameController
    {
        
    };

}

mono::IGameController& mono::GameControllerInstance()
{
    static GameController instance;
    return instance;
}




