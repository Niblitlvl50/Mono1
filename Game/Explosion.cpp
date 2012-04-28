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
    : mSprite("Explosion.sprite")
{
    mPosition = Math::Vector2f(100.0f, 100.0f);
    mScale = 20.0f;
    
    const mono::FrameDurations durations(7, 50);
    mSprite.DefineAnimation(constants::EXPLODE, 0, 6, durations);
    mSprite.SetAnimation(constants::EXPLODE);
}

void Explosion::Update(unsigned int delta)
{
    mSprite.doUpdate(delta);
}

void Explosion::Draw() const
{
    mSprite.Draw();
}
