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
#include "EntityBase.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "Weapons/IWeaponSystem.h"
#include "Weapons/WeaponFactory.h"

#include "Math/MathFunctions.h"
#include "Math/Matrix.h"
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

class SpriteEntity : public mono::EntityBase
{
public:

    SpriteEntity(const char* sprite_file)
    {
        m_sprite = mono::CreateSprite(sprite_file);
    }

    virtual void Draw(mono::IRenderer& renderer) const
    {
        renderer.DrawSprite(*m_sprite);
    }

    virtual void Update(unsigned int delta)
    {
        m_sprite->doUpdate(delta);
    }

    void SetAnimation(int animation_id)
    {
        m_sprite->SetAnimation(animation_id);
    }

    mono::ISpritePtr m_sprite;
};


Shuttle::Shuttle(float x, float y, mono::EventHandler& eventHandler)
    : mController(this, eventHandler),
      mEventHandler(eventHandler),
      m_fire(false)
{
    mPosition = math::Vector(x, y);
    mScale = math::Vector(20.0f, 20.0f);
    
    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(10.0f, INFINITY);
    mPhysicsObject.body->SetPosition(mPosition);
    mPhysicsObject.body->SetCollisionHandler(this);

    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(mPhysicsObject.body, mScale.x, mScale.y);
    shape->SetElasticity(0.1f);
    
    mPhysicsObject.shapes.push_back(shape);

    mSprite = mono::CreateSprite("sprites/shuttle.sprite");
    mSprite->SetAnimation(constants::IDLE);

    m_left_booster = std::make_shared<SpriteEntity>("sprites/booster.sprite");
    m_left_booster->SetScale(math::Vector(0.5f, 0.5f));
    m_left_booster->SetRotation(-math::PI_2());
    m_left_booster->SetPosition(math::Vector(-0.6f, 0.0f));

    m_right_booster = std::make_shared<SpriteEntity>("sprites/booster.sprite");
    m_right_booster->SetScale(math::Vector(0.5f, 0.5f));
    m_right_booster->SetRotation(math::PI_2());
    m_right_booster->SetPosition(math::Vector(0.6f, 0.0f));
    
    AddChild(m_left_booster);
    AddChild(m_right_booster);

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
    //mono::ListenerPosition(mPosition);

    constexpr math::Vector bullet_offset(0.0f, 15.0f);

    if(m_fire)
        mWeapon->Fire(mPosition + bullet_offset, mRotation);
}

void Shuttle::OnCollideWith(const mono::IBodyPtr& body)
{ }

void Shuttle::OnPostStep()
{ }

void Shuttle::SelectWeapon(WeaponType weapon)
{
    mWeapon = Factory::CreateWeapon(weapon, mEventHandler);
}

void Shuttle::ApplyRotationForce(float force)
{
    const math::Vector forceVector(force, 0.0);

    // First apply the rotational force at an offset of 20 in y axis, then negate the vector
    // and apply it to zero to counter the movement when we only want rotation.
    mPhysicsObject.body->ApplyForce(forceVector, math::Vector(0, 20));
    mPhysicsObject.body->ApplyForce(forceVector * -1, math::zeroVec);
}

void Shuttle::ApplyThrustForce(float force)
{
    const float rotation = Rotation();
    const math::Vector unit(-std::sin(rotation), std::cos(rotation));

    mPhysicsObject.body->ApplyForce(unit * force, math::zeroVec);
}

void Shuttle::ApplyImpulse(const math::Vector& force)
{
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

void Shuttle::SetBoosterThrusting(BoosterPosition position, bool enable)
{
    const int state = enable ? constants::THRUSTING : constants::IDLE;

    switch(position)
    {
        case BoosterPosition::LEFT:
            m_left_booster->SetAnimation(state);
            break;
        case BoosterPosition::RIGHT:
            m_right_booster->SetAnimation(state);
            break;
        case BoosterPosition::MAIN:
            mSprite->SetAnimation(state);
            break;
        case BoosterPosition::ALL:
            m_left_booster->SetAnimation(state);
            m_right_booster->SetAnimation(state);
            mSprite->SetAnimation(state);
            break;
    }
}



