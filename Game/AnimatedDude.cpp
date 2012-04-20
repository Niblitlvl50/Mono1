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
    : mPosition(x, y),
      mTarget(x, y),
      mSprite("ryu.png", 3, 4, 20.0f, 20.0f),
      mController(*this)
{
    mono::FrameDurations runDurations;
    runDurations.push_back(100);
    runDurations.push_back(100);
    runDurations.push_back(100);
    mSprite.DefineAnimation(constants::RUNNING, 1, 3, runDurations);
    
    mono::FrameDurations jumpDurations;
    jumpDurations.push_back(100);
    jumpDurations.push_back(100);
    jumpDurations.push_back(100);
    jumpDurations.push_back(100);
    mSprite.DefineAnimation(constants::JUMPING, 4, 7, jumpDurations);

    mono::FrameDurations fightDurations;
    fightDurations.push_back(100);
    fightDurations.push_back(100);
    fightDurations.push_back(100);
    fightDurations.push_back(100);
    mSprite.DefineAnimation(constants::FIGHTING, 8, 11, fightDurations);
    
    mSprite.SetAnimation(constants::RUNNING);
}

void AnimatedDude::Draw() const
{
    mSprite.DrawAt(mPosition.mX, mPosition.mY);
    EntityBase::Draw();
}

void AnimatedDude::Update(unsigned int delta)
{
    const float value = (delta * constants::SPEED);
    
    const float xzero = std::floor(std::abs(mPosition.mX - mTarget.mY));
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
    
    mSprite.Update(delta);
    
    EntityBase::Update(delta);
}
