//
//  FireBullet.cpp
//  Mono1
//
//  Created by Niblit on 2013-03-30.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "FireBullet.h"
#include "IRenderer.h"

using namespace game;

FireBullet::FireBullet(const math::Vector2f& start, float rotation, const math::Vector2f& speed)
    : mSprite("FireBullet.sprite"),
      mSpeed(speed)
{
    mPosition = start;
    mRotation = rotation;
    mScale = 15.0f;
}

void FireBullet::Update(unsigned int delta)
{
    mSprite.doUpdate(delta);
    mPosition += (mSpeed * delta * 0.001f);
}

void FireBullet::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
}