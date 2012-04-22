//
//  TestZone.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-06.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TestZone.h"
#include "TriangleObject.h"
#include "OscillatingLine.h"
#include "AnimatedDude.h"
#include "Explosion.h"

#include "SysTime.h"

using namespace game;

TestZone::TestZone()
    : mStartTime(Time::GetMilliseconds())
{
    AddEntityToLayer(BACKGROUND, mono::IEntityPtr(new TriangleObject));
    AddEntityToLayer(FOREGROUND, mono::IEntityPtr(new OscillatingLine));
    AddEntityToLayer(MIDDLEGROUND, mono::IEntityPtr(new AnimatedDude(100.0f, 50.0f)));
    AddEntityToLayer(FOREGROUND, mono::IEntityPtr(new Explosion));
}

bool TestZone::IsDone() const
{
    return false;
    
    
    const unsigned int elapsedTime = Time::GetMilliseconds() - mStartTime;
    if(elapsedTime > 5000)
        return true;
    
    return false;
}
