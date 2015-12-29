/*
 *  SquareObject.cpp
 *  Monolith1
 *
 *  Created by Niblit on 2011-02-21.
 *  Copyright 2011 __MyCompanyName__. All rights reserved.
 *
 */

#include "InvaderEntity.h"
#include "IRenderer.h"
#include "MathFunctions.h"

using namespace game;

InvaderEntity::InvaderEntity(float x, float y)
    : mSprite("invader.sprite"),
      mColor(0.0f, 0.5f, 0.5f)
{
    mPosition = math::Vector2f(x, y);
}

void InvaderEntity::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
}

void InvaderEntity::Update(unsigned int delta)
{
    constexpr float TWO_PI = math::PI() * 2.0;

    mRotation -= (delta * 0.001f);
    if(mRotation < -TWO_PI)
        mRotation += TWO_PI;

    mColor.hue += (delta * 0.0005f);
    if(mColor.hue > 1.0f)
        mColor.hue = 0.0f;

    mSprite.SetShade(mono::Color::ToRGBA(mColor));
}
