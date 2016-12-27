
#include "Enemy.h"

#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"

#include "Physics/IBody.h"
#include "Physics/IShape.h"
#include "Physics/CMFactory.h"

#include "IRenderer.h"

using namespace game;

Enemy::Enemy(EnemySetup& setup)
{
    mPosition = setup.position;
    mScale = math::Vector(setup.size, setup.size);

    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(setup.mass, 1.0f);
    mPhysicsObject.shapes.push_back(mono::PhysicsFactory::CreateShape(mPhysicsObject.body, setup.size / 2.0f, math::zeroVec));

    mPhysicsObject.body->SetPosition(mPosition);

    m_sprite = mono::CreateSprite(setup.sprite_file);

    m_controller = std::move(setup.controller);
    m_controller->Initialize(this);
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
