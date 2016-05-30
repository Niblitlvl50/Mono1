//
//  Explosion.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Explosion.h"
#include "IRenderer.h"

using namespace game;

Explosion::Explosion(const math::Vector2f& position, float scale, float rotation)
    : mSprite("explosion.sprite"),
      mRemoveMe(false)
{
    mPosition = position;
    mScale = math::Vector2f(scale, scale);
    mRotation = rotation;

    const auto func = [this] {
        mRemoveMe = true;
    };
    
    mSprite.SetAnimation(0, func);
}

void Explosion::Update(unsigned int delta)
{
    mSprite.doUpdate(delta);
}

void Explosion::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
}

bool Explosion::RemoveMe() const
{
    return mRemoveMe;
}
