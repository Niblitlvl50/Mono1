/*
 *  SquareObject.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SquareObject.h"
#include "SysOpenGL.h"

#include <string>

using namespace game;

SquareObject::SquareObject(float x, float y)
    : mSprite("Invader.sprite")
{
    mPosition = Math::Vector2f(x, y);
    //mRotationCenter = Math::Vector2f(-1.0f, -1.0f);
}

void SquareObject::Draw() const
{
    mSprite.Draw();
}

void SquareObject::Update(unsigned int delta)
{
    mRotation -= (delta * 0.1f);
    if(mRotation < -360.0f)
        mRotation += 360.0f;
}
