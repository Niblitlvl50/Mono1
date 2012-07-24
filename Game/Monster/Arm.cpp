//
//  Arm.cpp
//  Mono1
//
//  Created by Niblit on 2012-05-01.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Arm.h"
#include "SysOpenGL.h"

using namespace game;

namespace
{
    struct UpperArm : mono::EntityBase
    {
        UpperArm()
            : mSprite("Monster/arm_upper_left.lua")
        {
            mPosition = Math::Vector2f(0.0f, 0.0f);
            mScale = 1.0f;
        }
        
        void Draw(mono::IRenderer&) const
        {
            mSprite.Draw();
        }
        
        void Update(unsigned int delta)
        { }
        
        mono::Sprite mSprite;
    };
    
    struct LowerArm : mono::EntityBase
    {
        LowerArm()
            : mSprite("Monster/arm_left.lua")
        {
            mPosition = Math::Vector2f(0.0f, -1.0f);
            mRotation = 90.0f;
            mScale = 1.0f;
        }
        
        void Draw(mono::IRenderer&) const
        {
            mSprite.Draw();
        }
        
        void Update(unsigned int delta)
        { }
        
        mono::Sprite mSprite;
    };
    
    struct Hand : mono::EntityBase
    {
        Hand()
        : mSprite("Monster/hand_left.lua")
        {
            mPosition = Math::Vector2f(0.0f, -2.0f);
            mScale = 1.0f;
        }
        
        void Draw(mono::IRenderer&) const
        {
            mSprite.Draw();
        }
        
        void Update(unsigned int delta)
        { }
        
        mono::Sprite mSprite;
    };    
}

Arm::Arm(float x, float y)
    : mUpper(new UpperArm),
      mLower(new LowerArm),
      mHand(new Hand)
{
    AddChild(mUpper);
    AddChild(mLower);
    AddChild(mHand);
    
    mPosition = Math::Vector2f(x, y);
    //mScale = 0.8f;
}

void Arm::Draw(mono::IRenderer&) const
{ }

void Arm::Update(unsigned int delta)
{ }
