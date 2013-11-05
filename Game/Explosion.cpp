//
//  Explosion.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Explosion.h"
#include "IRenderer.h"
#include <functional>

using namespace game;

Explosion::Explosion(const math::Vector2f& position)
    : mSprite("explosion.sprite"),
      mRemoveMe(false)
{
    mPosition = position;
    mScale = math::Vector2f(20.0f, 20.0f);
    mSprite.SetAnimation(0, std::bind(&Explosion::OnFinished, this));
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

void Explosion::OnFinished()
{
    mRemoveMe = true;
}
