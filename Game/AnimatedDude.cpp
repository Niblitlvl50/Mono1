//
//  AnimatedDude.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "AnimatedDude.h"
#include "SysTime.h"

using namespace game;

namespace constants
{
    const float SPEED = 0.1f;
    
    enum
    {
        RUNNING = 0,
        JUMPING,
        FIGHTING
    };
}

AnimatedDude::AnimatedDude(float x, float y)
    : mTarget(x, y),
      mSprite("ryu.png", 3, 4),
      mController(*this)
{
    mPosition = Math::Vector2f(x, y);
    mScale = 20.0f;
    
    const mono::FrameDurations runDurations(3, 100);
    mSprite.DefineAnimation(constants::RUNNING, 1, 3, runDurations);
    
    const mono::FrameDurations jumpDurations(4, 100);
    mSprite.DefineAnimation(constants::JUMPING, 4, 7, jumpDurations);

    const mono::FrameDurations fightDurations(4, 100);
    mSprite.DefineAnimation(constants::FIGHTING, 8, 11, fightDurations);
    
    mSprite.SetAnimation(constants::FIGHTING);
}

void AnimatedDude::Draw() const
{
    mSprite.Draw();
}

void AnimatedDude::Update(unsigned int delta)
{
    const float value = (delta * constants::SPEED);
    
    const float xzero = std::floor(std::abs(mPosition.mX - mTarget.mX));
    const float yzero = std::floor(std::abs(mPosition.mY - mTarget.mY));
    
    // This shit is not working!
    
    if(xzero != 0.0f)
    {
        if(mPosition.mX > mTarget.mX)
            mPosition.mX -= value;
        else if(mPosition.mX < mTarget.mX)
            mPosition.mX += value;
    }
    
    if(yzero != 0.0f)
    {
        if(mPosition.mY > mTarget.mY)
            mPosition.mY -= value;
        else if(mPosition.mY < mTarget.mY)
            mPosition.mY += value;
    }
    
    mSprite.doUpdate(delta);
}
