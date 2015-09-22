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

using namespace game;

InvaderEntity::InvaderEntity(float x, float y)
    : mSprite("invader.sprite")
{
    mPosition = math::Vector2f(x, y);
    mSprite.SetShade(mono::Color::RGBA(1, 1, 1, 0.5));
}

void InvaderEntity::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
}

void InvaderEntity::Update(unsigned int delta)
{
    mRotation -= (delta * 0.1f);
    if(mRotation < -360.0f)
        mRotation += 360.0f;
}
