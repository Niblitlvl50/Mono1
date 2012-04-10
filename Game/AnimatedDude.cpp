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
    const float SPEED = 0.005f;
    
    enum
    {
        RUNNING = 0,
        JUMPING,
        FIGHTING
    };
}

AnimatedDude::AnimatedDude(float x, float y)
    : mX(x),
      mY(y),
      mSprite("ryu.png", 3, 4, 0.8f, 0.8f)
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
    mSprite.DrawAt(mX, mY);
    EntityBase::Draw();
}

void AnimatedDude::Update(unsigned int delta)
{
    mX += (delta * constants::SPEED);
    
    if(mX > 15.0f)
        mX = -15.0f;
    
    mSprite.Update(delta);
    
	EntityBase::Update(delta);
}
