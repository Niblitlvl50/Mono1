
#include "Enemy.h"
#include "EntityProperties.h"
#include "Weapons/WeaponFactory.h"

#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "Physics/CMIBody.h"
#include "Physics/CMIShape.h"
#include "Physics/CMFactory.h"

#include "IRenderer.h"

using namespace game;

Enemy::Enemy(EnemySetup& setup, mono::EventHandler& event_handler)
    : m_eventHandler(event_handler)
{
    m_controller = std::move(setup.controller);
    
    SetProperty(EntityProperties::DAMAGABLE);

    mPosition = setup.position;
    mScale = math::Vector2f(setup.size, setup.size);

    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(setup.mass, 1.0f);
    mPhysicsObject.shapes.push_back(mono::PhysicsFactory::CreateShape(mPhysicsObject.body, setup.size, math::zeroVec));

    mPhysicsObject.body->SetPosition(mPosition);

    m_weapon = game::Factory::CreateWeapon(WeaponType::STANDARD, m_eventHandler);
    m_sprite = mono::CreateSprite(setup.sprite_file);
}

void Enemy::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*m_sprite);
}

void Enemy::Update(unsigned int delta)
{
    m_sprite->doUpdate(delta);
    m_controller->doUpdate(delta);
}
