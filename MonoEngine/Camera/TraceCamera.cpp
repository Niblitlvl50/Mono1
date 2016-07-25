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

#include <cmath>

using namespace mono;

namespace constants
{
    constexpr float SPEED = 0.01f;
}

TraceCamera::TraceCamera(int width, int height)
    : mViewport(0.0f, 0.0f, width, height),
      mTargetViewport(mViewport),
      m_offset(math::zeroVec)
{ }

void TraceCamera::Update(unsigned int delta)
{
    const float change = (mTargetViewport.mB.x - mViewport.mB.x);
    const float xzero = std::floor(std::abs(change));

    if(xzero != 0.0f)
    {
        const float aspect = mViewport.mB.x / mViewport.mB.y;
        math::ResizeQuad(mViewport, change * 0.1f, aspect);
    }
    
    if(mEntity)
    {
        const math::Vector2f& targetPosition = mEntity->Position() - (mViewport.mB * 0.5f);
        const math::Vector2f& diff = targetPosition - mViewport.mA;
    
        const math::Vector2f& move = diff * (delta * constants::SPEED);
        mViewport.mA += move;
    }
}

void TraceCamera::Follow(const mono::IEntityPtr& entity, const math::Vector2f& offset)
{
    mEntity = entity;
    m_offset = offset;
}

void TraceCamera::Unfollow()
{
    mEntity = nullptr;
}

math::Quad TraceCamera::GetViewport() const
{
    math::Quad result = mViewport;
    result.mA -= m_offset;

    return result;
}

void TraceCamera::SetViewport(const math::Quad& viewport)
{
    mViewport = viewport;
    mTargetViewport = viewport;
}

void TraceCamera::SetTargetViewport(const math::Quad& target)
{
    mTargetViewport = target;
}

void TraceCamera::SetPosition(const math::Vector2f& position)
{
    // The position is the middle of the quad, not the lower left corner.
    const math::Vector2f& xy = position - (mViewport.mB * 0.5f);
    mViewport.mA = xy;
}

math::Vector2f TraceCamera::GetPosition() const
{
    return mViewport.mA + (mViewport.mB * 0.5f);
}



