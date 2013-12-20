//
//  AnimatedDude.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "AnimatedDude.h"
#include "SysTime.h"
#include "IRenderer.h"
#include <cmath>

using namespace game;

namespace constants
{
    const float SPEED = 0.1f;
    
    enum
    {
        IDLE = 0,
        RUNNING,
        JUMPING,
        FIGHTING
    };
}

AnimatedDude::AnimatedDude(float x, float y, mono::EventHandler& eventHandler)
    : mTarget(x, y),
      mSprite("ryu.sprite"),
      mController(this, eventHandler)
{
    mPosition = math::Vector2f(x, y);
    mScale = math::Vector2f(30.0f, 30.0f);
        
    mSprite.SetAnimation(constants::IDLE);
    //mSprite.SetAnimation(constants::RUNNING);
    //mSprite.SetAnimation(constants::JUMPING);
    //mSprite.SetAnimation(constants::FIGHTING);
}

void AnimatedDude::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
    
    const math::Vector2f textPosition = mPosition + math::Vector2f(0.0f, 25.0f);
    mono::Color color = {0.5f, 1.0f, 0.0f, 1.0f};
    renderer.DrawText("Ryu Hayabusa", textPosition, true, color);
}

void AnimatedDude::Update(unsigned int delta)
{
    const float value = (delta * constants::SPEED);
    
    const bool xzero = (std::floor(std::abs(mPosition.mX - mTarget.mX)) == 0.0f);
    const bool yzero = (std::floor(std::abs(mPosition.mY - mTarget.mY)) == 0.0f);
        
    if(!xzero)
    {
        if(mPosition.mX > mTarget.mX)
            mPosition.mX -= value;
        else if(mPosition.mX < mTarget.mX)
            mPosition.mX += value;
    }
    
    if(!yzero)
    {
        if(mPosition.mY > mTarget.mY)
            mPosition.mY -= value;
        else if(mPosition.mY < mTarget.mY)
            mPosition.mY += value;
    }
    
    if(xzero && yzero)
        mSprite.SetAnimation(constants::IDLE);
    else
        mSprite.SetAnimation(constants::RUNNING);    
    
    mSprite.doUpdate(delta);
}
