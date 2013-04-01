/*
 *  SquareObject.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "SquareObject.h"
#include "IRenderer.h"

using namespace game;

SquareObject::SquareObject(float x, float y)
    : mSprite("invader.sprite")
{
    mPosition = math::Vector2f(x, y);
    //mRotationCenter = Math::Vector2f(-1.0f, -1.0f);
}

void SquareObject::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
}

void SquareObject::Update(unsigned int delta)
{
    mRotation -= (delta * 0.1f);
    if(mRotation < -360.0f)
        mRotation += 360.0f;
}
