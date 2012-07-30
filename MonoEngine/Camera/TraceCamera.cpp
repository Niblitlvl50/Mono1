//
//  TraceCamera.cpp
//  Mono1
//
//  Created by Niblit on 2012-05-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TraceCamera.h"
#include "IEntity.h"
#include "Vector2f.h"

using namespace mono;

namespace constants
{
    const float SPEED = 0.01f;
}

TraceCamera::TraceCamera(int width, int height)
    : mViewport(0.0f, 0.0f, width, height)
{ }

void TraceCamera::Update(unsigned int delta)
{
    if(!mEntity)
        return;
    
    //const Math::Vector2f size(mViewport.mW, mViewport.mH);
    const Math::Vector2f targetPosition = mEntity->Position() - (mViewport.mB * 0.5f);
    const Math::Vector2f diff = targetPosition - mViewport.mA;
    
    const Math::Vector2f move = diff * (delta * constants::SPEED);
    mViewport.mA += move;
}

void TraceCamera::Follow(const mono::IEntityPtr entity)
{
    mEntity = entity;
}

void TraceCamera::Unfollow()
{
    mEntity.reset();
}

const Math::Quad& TraceCamera::GetViewport() const
{
    return mViewport;
}
void TraceCamera::SetPosition(const Math::Vector2f& position)
{
    const Math::Vector2f xy = position - (mViewport.mB * 0.5f);
    mViewport.mA = xy;
}



