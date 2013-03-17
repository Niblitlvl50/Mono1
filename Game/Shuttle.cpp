//
//  Shuttle.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Shuttle.h"
#include "CMIBody.h"
#include "CMIShape.h"
#include "CMFactory.h"

#include "Meteor.h"
#include "EventHandler.h"
#include "SpawnEntityEvent.h"
#include "SpawnPhysicsEntityEvent.h"

using namespace game;

namespace constants
{    
    enum
    {
        IDLE = 0,
        THRUSTING
    };
}


Shuttle::Shuttle(float x, float y)
    : mSprite("shuttle.sprite"),
      mController(this)
{
    mPosition = Math::Vector2f(x, y);
    mScale = 20.0f;
    
    mPhysicsObject.body = cm::Factory::CreateBody(15.0f, 1.0f);
    cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, 5.0f, Math::Vector2f(0.0f, 0.0f), false);
    shape->SetElasticity(0.9f);
    
    mPhysicsObject.shapes.push_back(shape);    
    mPhysicsObject.body->SetPosition(mPosition);
    
    mSprite.SetAnimation(constants::THRUSTING);
}

void Shuttle::Draw(mono::IRenderer&) const
{
    mSprite.Draw();
}

void Shuttle::Update(unsigned int delta)
{
    mSprite.doUpdate(delta);

    mPosition = mPhysicsObject.body->GetPosition();
    mRotation = mPhysicsObject.body->GetAngle();
}

void Shuttle::Fire()
{
    //const game::SpawnEntityEvent event(mono::IEntityPtr(new Meteor(mPosition.mX, mPosition.mY)));

    std::tr1::shared_ptr<Meteor> entity(new Meteor(mPosition.mX, mPosition.mY));
    const game::SpawnPhysicsEntityEvent event(entity, entity->mPhysicsObject);  
    
    entity->mPhysicsObject.body->ApplyForce(Math::Vector2f(200.0f, 200.0f), Math::Vector2f(0.0, 0.0));
    
    mono::EventHandler::DispatchEvent(event);
}



