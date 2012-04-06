//
//  GameController.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "GameController.h"
#include "IZone.h"

#include <stdexcept>

using namespace mono;

GameController& GameController::Instance()
{
    static GameController instance;
    return instance;
}

GameController::GameController()
{ }

IZonePtr GameController::GetCurrentZone()
{
    if(mZoneStack.empty())
        throw std::runtime_error("Zone stack is empty, exit");

    IZonePtr zone = mZoneStack.top();
    if(zone->IsDone())
    {
        mZoneStack.pop();
        if(mZoneStack.empty())
            throw std::runtime_error("Zone stack is empty, exit");

        zone = mZoneStack.top();
    }
    
    return zone;
}

void GameController::InsertZone(IZonePtr zone)
{
    mZoneStack.push(zone);
}
