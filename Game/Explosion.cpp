//
//  Explosion.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Explosion.h"

using namespace game;

namespace constants
{
    const int EXPLODE = 1;
}

Explosion::Explosion()
    : mSprite("explosion.sprite")
{
    mPosition = Math::Vector2f(100.0f, 350.0f);
    mScale = 20.0f;
}

void Explosion::Update(unsigned int delta)
{
    mSprite.doUpdate(delta);
}

void Explosion::Draw(mono::IRenderer&) const
{
    mSprite.Draw();
}
