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
#include "MathFunctions.h"

using namespace mono;

namespace constants
{
    const float SPEED = 0.01f;
}

TraceCamera::TraceCamera(int width, int height)
    : mController(this),
      mViewport(0.0f, 0.0f, width, height),
      mTargetViewport(mViewport)
{ }

void TraceCamera::Update(unsigned int delta)
{
    // Add functionallity for smoth zoom here.
    const float change = (mTargetViewport.mB.mX - mViewport.mB.mX) * 0.1f;
    const float aspect = mViewport.mB.mX / mViewport.mB.mY;
    Math::ResizeQuad(mViewport, change, aspect);
    
    if(!mEntity)
        return;
    
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

Math::Quad& TraceCamera::GetViewport()
{
    return mViewport;
}

void TraceCamera::SetTargetViewport(const Math::Quad& target)
{
    mTargetViewport = target;
}

void TraceCamera::SetPosition(const Math::Vector2f& position)
{
    const Math::Vector2f xy = position - (mViewport.mB * 0.5f);
    mViewport.mA = xy;
}



