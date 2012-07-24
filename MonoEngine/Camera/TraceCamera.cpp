//
//  TraceCamera.cpp
//  Mono1
//
//  Created by Niblit on 2012-05-11.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TraceCamera.h"
#include "IEntity.h"

using namespace mono;

namespace constants
{
    const float SPEED = 0.01f;
}

TraceCamera::TraceCamera(int width, int height)
    : mSize(width, height)
{ }

void TraceCamera::Update(unsigned int delta)
{
    if(!mEntity)
        return;
    
    const Math::Vector2f& targetPosition = mEntity->Position() - (mSize * 0.5f);
    const Math::Vector2f diff = targetPosition - mPosition;
    
    const Math::Vector2f move = diff * (delta * constants::SPEED);
    mPosition += move;
}

void TraceCamera::Follow(const mono::IEntityPtr entity)
{
    mEntity = entity;
}

void TraceCamera::Unfollow()
{
    mEntity.reset();
}

const Math::Vector2f& TraceCamera::Position() const
{
    return mPosition;
}
void TraceCamera::SetPosition(const Math::Vector2f& position)
{
    mPosition = position - (mSize * 0.5f);
}

const Math::Vector2f& TraceCamera::Size() const
{
    return mSize;
}




