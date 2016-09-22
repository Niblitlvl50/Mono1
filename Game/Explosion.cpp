//
//  Explosion.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-21.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Explosion.h"
#include "IRenderer.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "Events/RemoveEntityEvent.h"
#include "EventHandler/EventHandler.h"

using namespace game;

Explosion::Explosion(mono::EventHandler& event_handler, const math::Vector2f& position, float scale, float rotation)
{
    mPosition = position;
    mScale = math::Vector2f(scale, scale);
    mRotation = rotation;

    const uint id = Id();

    const auto func = [&event_handler, id] {
        event_handler.DispatchEvent(game::RemoveEntityEvent(id));
    };
    
    mSprite = mono::CreateSprite("explosion.sprite");
    mSprite->SetAnimation(0, func);
}

void Explosion::Update(unsigned int delta)
{
    mSprite->doUpdate(delta);
}

void Explosion::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*mSprite);
}
