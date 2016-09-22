//
//  Shuttle.cpp
//  Mono1
//
//  Created by Niblit on 2012-08-25.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Shuttle.h"
#include "Physics/CMIBody.h"
#include "Physics/CMIShape.h"
#include "Physics/CMFactory.h"

#include "IRenderer.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "IWeaponSystem.h"
#include "WeaponFactory.h"

#include "Math/MathFunctions.h"
#include "Audio/AudioListener.h"

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
    : mController(this, eventHandler),
      mEventHandler(eventHandler),
      m_fire(false)
{
    mPosition = math::Vector2f(x, y);
    mScale = math::Vector2f(20.0f, 20.0f);
    
    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(10.0f, INFINITY);
    mPhysicsObject.body->SetPosition(mPosition);
    mPhysicsObject.body->SetCollisionHandler(this);

    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(mPhysicsObject.body, mScale.x, mScale.y);
    shape->SetElasticity(0.1f);
    
    mPhysicsObject.shapes.push_back(shape);

    mSprite = mono::CreateSprite("sprites/shuttle.sprite");
    mSprite->SetAnimation(constants::IDLE);

    // Make sure we have a weapon
    SelectWeapon(WeaponType::STANDARD);
}

void Shuttle::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*mSprite);
}

void Shuttle::Update(unsigned int delta)
{
    mSprite->doUpdate(delta);
    mono::ListenerPosition(mPosition);

    if(m_fire)
        mWeapon->Fire(mPosition, mRotation);
}

void Shuttle::OnCollideWith(const mono::IBodyPtr& body)
{
    //body->IsStatic();
    //mPhysicsObject.body->GetMoment();
}

void Shuttle::OnPostStep()
{ }

void Shuttle::SelectWeapon(WeaponType weapon)
{
    mWeapon = Factory::CreateWeapon(weapon, mEventHandler);

    // Setup the weapon's sprite
    //mWeaponSprite.
}

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
    const math::Vector2f unit(-std::sin(rotation), std::cos(rotation));

    mPhysicsObject.body->ApplyForce(unit * force, math::zeroVec);
}

void Shuttle::ApplyImpulse(const math::Vector2f& force)
{
    //mPhysicsObject.body->ApplyImpulse(force, math::zeroVec);
    mPhysicsObject.body->ApplyImpulse(force, mPosition);
}

void Shuttle::Fire()
{
    m_fire = true;
}

void Shuttle::StopFire()
{
    m_fire = false;
}

void Shuttle::StartThrusting()
{
    mSprite->SetAnimation(constants::THRUSTING);
}

void Shuttle::StopThrusting()
{
    mSprite->SetAnimation(constants::IDLE);
}



