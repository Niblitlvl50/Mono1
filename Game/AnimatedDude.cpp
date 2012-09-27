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

AnimatedDude::AnimatedDude(float x, float y)
    : mTarget(x, y),
      mSprite("Ryu.sprite"),
      mController(*this)
{
    mPosition = Math::Vector2f(x, y);
    mScale = 30.0f;
        
    mSprite.SetAnimation(constants::RUNNING);
}

void AnimatedDude::Draw(mono::IRenderer& renderer) const
{
    mSprite.Draw();
    
    const Math::Vector2f textPosition = mPosition + Math::Vector2f(0.0f, 25.0f);
    Color color = {0.5f, 1.0f, 0.0f, 1.0f};
    renderer.DrawText("Ryu Hayabusa", textPosition, true, color);
}

void AnimatedDude::Update(unsigned int delta)
{
    const float value = (delta * constants::SPEED);
    
    const float xzero = std::floor(std::abs(mPosition.mX - mTarget.mX));
    const float yzero = std::floor(std::abs(mPosition.mY - mTarget.mY));
        
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
