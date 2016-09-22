//
//  FireBullet.cpp
//  Mono1
//
//  Created by Niblit on 2013-03-30.
//  Copyright (c) 2013 __MyCompanyName__. All rights reserved.
//

#include "FireBullet.h"
#include "IRenderer.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "Events/RemoveEntityEvent.h"
#include "Events/DamageEvent.h"
#include "EventHandler/EventHandler.h"

#include "Physics/CMIBody.h"
#include "Physics/CMIShape.h"
#include "Physics/CMFactory.h"

using namespace game;

FireBullet::FireBullet(const math::Vector2f& start, float rotation, mono::EventHandler& eventHandler)
    : mEventHandler(eventHandler)
{
    dead = false;

    mScale = math::Vector2f(15.0f, 15.0f);
    
    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(1.0f, 1.0f);
    mPhysicsObject.body->SetPosition(start);  
    mPhysicsObject.body->SetAngle(rotation);
    mPhysicsObject.body->SetCollisionHandler(this);
    
    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(mPhysicsObject.body, 5.0f, math::Vector2f(0.0f, 0.0f));
    
    mPhysicsObject.body->SetMoment(shape->GetInertiaValue());    
    mPhysicsObject.shapes.push_back(shape);        

    mSprite = mono::CreateSprite("firebullet.sprite");
}

void FireBullet::Update(unsigned int delta)
{
    mSprite->doUpdate(delta);
}

void FireBullet::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*mSprite);
}

void FireBullet::OnCollideWith(const mono::IBodyPtr& body)
{
    mEventHandler.DispatchEvent(game::DamageEvent(body, 20));
}

void FireBullet::OnPostStep()
{
    if(dead)
        return;

    mEventHandler.DispatchEvent(game::RemoveEntityEvent(Id()));
    dead = true;
}





