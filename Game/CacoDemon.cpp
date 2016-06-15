//
//  CacoDemon.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 22/11/15.
//
//

#include "CacoDemon.h"
#include "IRenderer.h"
#include "WeaponFactory.h"
#include "CMIBody.h"
#include "CMIShape.h"
#include "CMFactory.h"

#include "EntityProperties.h"

using namespace game;

namespace
{
    enum Animation
    {
        IDLE,
        ATTACK,
        HURT,
        DEATH
    };
}

CacoDemon::CacoDemon(mono::EventHandler& eventHandler)
    : m_eventHandler(eventHandler),
      m_sprite("cacodemon.sprite"),
      m_controller(this)
{
    mPosition = math::Vector2f(0, -200);
    mScale = math::Vector2f(100.0f, 100.0f);

    SetProperty(EntityProperties::DAMAGABLE);

    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(500.0f, 1.0f);
    mPhysicsObject.body->SetPosition(mPosition);

    mPhysicsObject.body->SetCollisionHandler(&m_controller);

    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(mPhysicsObject.body, 80, 80);
    shape->SetElasticity(0.1f);

    mPhysicsObject.body->SetMoment(shape->GetInertiaValue());
    mPhysicsObject.shapes.push_back(shape);

    m_sprite.SetAnimation(ATTACK);

    m_weapon = Factory::CreateWeapon(game::WeaponType::CACOPLASMA, m_eventHandler);
}

void CacoDemon::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(m_sprite);
}

void CacoDemon::Update(unsigned int delta)
{
    m_sprite.doUpdate(delta);
    m_weapon->Fire(mPosition, mRotation);
}
