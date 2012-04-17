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
        
        virtual void InsertZone(IZonePtr zone) = 0;
        virtual IZonePtr GetCurrentZone() = 0;
        
        virtual void SetWindowSize(const Math::Vector2f& size) = 0;
        virtual const Math::Vector2f& GetWindowSize() const = 0;
    };
    
    IGameController& GameControllerInstance();
}

