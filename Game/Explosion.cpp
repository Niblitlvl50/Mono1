//
//  Explosion.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Explosion.h"
#include "IRenderer.h"
#include "RemoveEntityEvent.h"
#include "EventHandler.h"

using namespace game;

Explosion::Explosion(mono::EventHandler& event_handler, const math::Vector2f& position, float scale, float rotation)
    : mSprite("explosion.sprite")
{
    mPosition = position;
    mScale = math::Vector2f(scale, scale);
    mRotation = rotation;

    const uint id = Id();

    const auto func = [&event_handler, id] {
        event_handler.DispatchEvent(game::RemoveEntityByIdEvent(id));
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
