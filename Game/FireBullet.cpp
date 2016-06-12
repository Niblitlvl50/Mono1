//
//  FireBullet.cpp
//  Mono1
//
//  Created by Niblit on 2013-03-30.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "FireBullet.h"
#include "IRenderer.h"
#include "Explosion.h"
#include "SpawnEntityEvent.h"
#include "RemoveEntityEvent.h"
#include "DamageEvent.h"
#include "EventHandler.h"

#include "CMIBody.h"
#include "CMIShape.h"
#include "CMFactory.h"

#include "Math.h"

#include "Utils.h"

using namespace game;

FireBullet::FireBullet(const math::Vector2f& start, float rotation, mono::EventHandler& eventHandler)
    : mSprite("firebullet.sprite"),
      mEventHandler(eventHandler)
{
    dead = false;

    mScale = math::Vector2f(15.0f, 15.0f);
    
    mPhysicsObject.body = cm::Factory::CreateBody(1.0f, 1.0f);
    mPhysicsObject.body->SetPosition(start);  
    mPhysicsObject.body->SetAngle(rotation);
    mPhysicsObject.body->SetCollisionHandler(this);
    
    cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, 5.0f, math::Vector2f(0.0f, 0.0f));
    
    mPhysicsObject.body->SetMoment(shape->GetInertiaValue());    
    mPhysicsObject.shapes.push_back(shape);        
}

void FireBullet::Update(unsigned int delta)
{
    mSprite.doUpdate(delta);
}

void FireBullet::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
}

void FireBullet::OnCollideWith(const cm::IBodyPtr& body)
{
    mEventHandler.DispatchEvent(game::DamageEvent(body, 20));
}

void FireBullet::OnPostStep()
{
    if(dead)
        return;

    const float rotation = mono::Random() * 360.0;
    const game::SpawnEntityEvent event(std::make_shared<Explosion>(mEventHandler, mPosition, 20, rotation));
    mEventHandler.DispatchEvent(event);

    mEventHandler.DispatchEvent(game::RemoveEntityByIdEvent(Id()));

    dead = true;
}





