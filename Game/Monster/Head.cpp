//
//  Head.cpp
//  Mono1
//
//  Created by Niblit on 2012-05-01.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Head.h"
#include "SysOpenGL.h"

using namespace game;

Head::Head(float x, float y)
    : mHead("Monster/head.lua"),
      mJaw("Monster/jaw.lua")
{
    mPosition = Math::Vector2f(x, y);
    mScale = 0.7f;
}

void Head::Draw() const
{
    mHead.Draw();
    
    glTranslatef(0.0f, -0.8f, 0.0f);
    mJaw.Draw();
}

void Head::Update(unsigned int delta)
{

}

