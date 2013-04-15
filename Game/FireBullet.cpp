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

#include "MathFwd.h"

using namespace game;

FireBullet::FireBullet(const math::Vector2f& start, float rotation)
    : mSprite("firebullet.sprite"),
      mRemoveMe(false)
{
    mScale = 15.0f;
    
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
    const game::SpawnEntityEvent event(mono::IEntityPtr(new Explosion(mPosition)));
    mono::EventHandler::DispatchEvent(event);

    mRemoveMe = true;
}

void FireBullet::OnPostStep()
{
    //mRemoveMe = true;
}





