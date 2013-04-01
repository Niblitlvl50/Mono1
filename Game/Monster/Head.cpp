//
//  Head.cpp
//  Mono1
//
//  Created by Niblit on 2012-05-01.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Head.h"
#include "SysOpenGL.h"
#include "IRenderer.h"

using namespace game;

Head::Head(float x, float y)
    : mHead("Monster/head.lua"),
      mJaw("Monster/jaw.lua")
{
    mPosition = math::Vector2f(x, y);
    mScale = 0.7f;
}

void Head::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mHead);
    
    glTranslatef(0.0f, -0.8f, 0.0f);
    renderer.DrawSprite(mJaw);
}

void Head::Update(unsigned int delta)
{

}

