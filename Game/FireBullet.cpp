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
#include "EventHandler.h"

#include "CMIBody.h"
#include "CMIShape.h"
#include "CMFactory.h"

#include "Math.h"

#include "Utils.h"

using namespace game;

FireBullet::FireBullet(const math::Vector2f& start, float rotation, mono::EventHandler& eventHandler)
    : mSprite("firebullet.sprite"),
      mEventHandler(eventHandler),
      mRemoveMe(false)
{
    mScale = math::Vector2f(15.0f, 15.0f);
    
    mPhysicsObject.body = cm::Factory::CreateBody(1.0f, 1.0f);
    mPhysicsObject.body->SetPosition(start);  
    mPhysicsObject.body->SetAngle(rotation);
    mPhysicsObject.body->SetCollisionHandler(this);
    
    cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, 7.0f, math::Vector2f(0.0f, 0.0f));
    
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

bool FireBullet::RemoveMe() const
{
    return mRemoveMe;
}

void FireBullet::OnCollideWith(cm::IBodyPtr body)
{
    const float rotation = mono::Random() * 360.0;
    const game::SpawnEntityEvent event(std::make_shared<Explosion>(mPosition, 20, rotation));
    mEventHandler.DispatchEvent(event);

    mRemoveMe = true;
}

void FireBullet::OnPostStep()
{
    //mRemoveMe = true;
}





