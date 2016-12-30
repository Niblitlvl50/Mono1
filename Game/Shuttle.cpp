
#include "Shuttle.h"
#include "Physics/IBody.h"
#include "Physics/IShape.h"
#include "Physics/CMFactory.h"
#include "Utils.h"

#include "AIKnowledge.h"
#include "EntityProperties.h"
#include "CollisionConfiguration.h"

#include "IRenderer.h"
#include "EntityBase.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "Weapons/IWeaponSystem.h"
#include "Weapons/WeaponFactory.h"

#include "Math/MathFunctions.h"
#include "Math/Matrix.h"

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


Shuttle::Shuttle(const math::Vector& position, mono::EventHandler& eventHandler)
    : m_controller(this, eventHandler),
      m_event_handler(eventHandler),
      m_fire(false)
{
    mPosition = position;
    mScale = math::Vector(20.0f, 20.0f);
    
    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(10.0f, INFINITY);
    mPhysicsObject.body->SetPosition(mPosition);
    mPhysicsObject.body->SetCollisionHandler(this);

    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(mPhysicsObject.body, mScale.x, mScale.y);
    shape->SetElasticity(0.1f);
    shape->SetCollisionFilter(CollisionCategory::PLAYER, PLAYER_MASK);
    
    mPhysicsObject.shapes.push_back(shape);

    m_sprite = mono::CreateSprite("sprites/shuttle.sprite");
    m_sprite->SetAnimation(constants::IDLE);

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

    SetProperty(EntityProperties::DAMAGABLE);
    
    // Make sure we have a weapon
    SelectWeapon(WeaponType::STANDARD);
}

void Shuttle::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*m_sprite);
}

void Shuttle::Update(unsigned int delta)
{
    m_sprite->doUpdate(delta);

    if(m_fire)
    {
        const float x_shift = mono::Random() * 12.0f - 6.0f;
        m_weapon->Fire(mPosition + math::Vector(x_shift, 0.0f), mRotation);
    }

    game::player_position = mPosition;
}

void Shuttle::OnCollideWith(const mono::IBodyPtr& body)
{ }

void Shuttle::OnPostStep()
{ }

void Shuttle::SelectWeapon(WeaponType weapon)
{
    m_weapon = Factory::CreateWeapon(weapon, WeaponFaction::PLAYER, m_event_handler);
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
            m_sprite->SetAnimation(state);
            break;
        case BoosterPosition::ALL:
            m_left_booster->SetAnimation(state);
            m_right_booster->SetAnimation(state);
            m_sprite->SetAnimation(state);
            break;
    }
}



