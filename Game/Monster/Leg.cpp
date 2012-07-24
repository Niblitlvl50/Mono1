//
//  Leg.cpp
//  Mono1
//
//  Created by Niblit on 2012-05-01.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Leg.h"


using namespace game;

Leg::Leg(float x, float y)
    : mUpper("Monster/leg_upper_right.lua"),
      mLower("Monster/leg_right.lua"),
      mFoot("Monster/foot_right.lua")
{
    mPosition = Math::Vector2f(x, y);
}

void Leg::Draw(mono::IRenderer&) const
{
    mUpper.Draw();
    mLower.Draw();
    mFoot.Draw();
}

void Leg::Update(unsigned int delta)
{

}
