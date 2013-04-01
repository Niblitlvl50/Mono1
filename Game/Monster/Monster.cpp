//
//  Monster.cpp
//  Mono1
//
//  Created by Niblit on 2012-05-01.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Monster.h"
#include "Head.h"
#include "Arm.h"
#include "Leg.h"

#include "IRenderer.h"


using namespace game;

Monster::Monster()
    : mBody("Monster/body.lua"),
      mHead(new Head(-0.4f, 1.6f)),
      mRightArm(new Arm(2.0f, 0.0f)),
      mLeftArm(new Arm(-2.0f, -0.5f)),
      mRightLeg(new Leg(2.0f, -4.0f)),
      mLeftLeg(new Leg(-2.0f, -4.0f))
{
    AddChild(mHead);
    //AddChild(mRightArm);
    AddChild(mLeftArm);
    //AddChild(mRightLeg);
    //AddChild(mLeftLeg);
    
    mPosition = math::Vector2f(200.0f, 100.0f);
    mScale = 30.0f;
}

void Monster::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mBody);
}

void Monster::Update(unsigned int delta)
{

}
