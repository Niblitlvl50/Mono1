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
#include "IRenderer.h"

#include "SysTime.h"

using namespace game;

TestZone::TestZone()
    : mStartTime(Time::GetMilliseconds())
{
    mEntities.push_back(mono::IEntityPtr(new TriangleObject));
    mEntities.push_back(mono::IEntityPtr(new OscillatingLine));
}

void TestZone::Accept(mono::IRenderer& renderer)
{
    for(mono::IEntityCollection::iterator it = mEntities.begin(), end = mEntities.end(); it != end; ++it)
    {
        mono::IEntityPtr entity = *it;
        renderer.AddEntity(entity);
    }
}

bool TestZone::IsDone() const
{
    return false;
    
    
    const unsigned int elapsedTime = Time::GetMilliseconds() - mStartTime;
    if(elapsedTime > 5000)
        return true;
    
    return false;
}
