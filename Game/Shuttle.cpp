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
#include "FireBullet.h"
#include "EventHandler.h"
#include "SpawnEntityEvent.h"
#include "SpawnPhysicsEntityEvent.h"
#include "IRenderer.h"

#include "MathFunctions.h"
#include <cmath>

using namespace game;

namespace constants
{    
    enum
    {
        IDLE = 0,
        THRUSTING
    };
}


Shuttle::Shuttle(float x, float y, mono::EventHandler& eventHandler)
    : mSprite("shuttle.sprite"),
      mController(this, eventHandler),
      mEventHandler(eventHandler)
{
    mPosition = math::Vector2f(x, y);
    mScale = 20.0f;
    
    mPhysicsObject.body = cm::Factory::CreateBody(10.0f, 1.0f);
    mPhysicsObject.body->SetPosition(mPosition);
    mPhysicsObject.body->SetCollisionHandler(this);

    cm::IShapePtr shape = cm::Factory::CreateShape(mPhysicsObject.body, mScale, mScale);
    shape->SetElasticity(0.1f);
    
    mPhysicsObject.body->SetMoment(shape->GetInertiaValue());
    mPhysicsObject.shapes.push_back(shape);    
    
    mSprite.SetAnimation(constants::IDLE);
}

void Shuttle::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(mSprite);
}

void Shuttle::Update(unsigned int delta)
{
    mSprite.doUpdate(delta);
}

void Shuttle::OnCollideWith(cm::IBodyPtr body)
{
    body->IsStatic();
    mPhysicsObject.body->GetMoment();
}

void Shuttle::OnPostStep()
{ }

void Shuttle::ApplyRotationForce(float force)
{
    const math::Vector2f forceVector(force, 0.0);

    // First apply the rotational force at an offset of 20 in y axis, then negate the vector
    // and apply it to zero to counter the movement when we only want rotation.
    mPhysicsObject.body->ApplyForce(forceVector, math::Vector2f(0, 20));
    mPhysicsObject.body->ApplyForce(forceVector * -1, math::zeroVec);
}

void Shuttle::ApplyThrustForce(float force)
{
    const float rotation = Rotation();
    const math::Vector2f unit(-std::sin(math::DegToRad(rotation)),
                               std::cos(math::DegToRad(rotation)));

    mPhysicsObject.body->ApplyForce(unit * force, math::zeroVec);
}

void Shuttle::Fire()
{
    const math::Vector2f unit(-std::sin(math::DegToRad(mRotation)),
                               std::cos(math::DegToRad(mRotation)));
    const math::Vector2f& position = mPosition + (unit * 20.0f);
    const math::Vector2f& impulse = unit * 500.0f;
    
    auto bullet = std::make_shared<FireBullet>(position, mRotation, mEventHandler);
    bullet->GetPhysics().body->ApplyImpulse(impulse, math::zeroVec);

    mEventHandler.DispatchEvent(game::SpawnPhysicsEntityEvent(bullet));
}

void Shuttle::StartThrusting()
{
    mSprite.SetAnimation(constants::THRUSTING);
}

void Shuttle::StopThrusting()
{
    mSprite.SetAnimation(constants::IDLE);
}



