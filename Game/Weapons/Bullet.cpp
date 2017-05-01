
#include "Bullet.h"
#include "CollisionConfiguration.h"

#include "Physics/IBody.h"
#include "Physics/IShape.h"
#include "Physics/CMFactory.h"

#include "Rendering/IRenderer.h"
#include "Rendering/Sprite/ISprite.h"
#include "Rendering/Sprite/SpriteFactory.h"
#include "Audio/ISound.h"
#include "Audio/AudioFactory.h"
#include "Random.h"

using namespace game;

Bullet::Bullet(const BulletConfiguration& config)
    : m_collisionCallback(config.collision_callback)
{
    mScale = math::Vector(1.0f, 1.0f) * config.scale;

    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(1.0f, 1.0f);
    mPhysicsObject.body->SetCollisionHandler(this);

    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(
        mPhysicsObject.body,
        config.collision_radius * config.scale,
        math::Vector(0.0f, 0.0f)
    );

    shape->SetCollisionFilter(config.collision_category, config.collision_mask);

    mPhysicsObject.body->SetMoment(shape->GetInertiaValue());
    mPhysicsObject.shapes.push_back(shape);

    m_sprite = mono::CreateSprite(config.sprite_file);
    m_sprite->SetShade(config.shade);

    if(config.sound_file)
    {
        m_sound = mono::AudioFactory::CreateSound(config.sound_file, true, false);
        m_sound->Play();
    }

    const float life_span = config.life_span + (mono::Random() * config.fuzzy_life_span);
    m_lifeSpan = life_span * 1000.0f;
}

void Bullet::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*m_sprite);
}

void Bullet::Update(unsigned int delta)
{
    m_sprite->doUpdate(delta);

    if(m_sound)
        m_sound->Position(mPosition.x, mPosition.y);

    m_lifeSpan -= delta;
    if(m_lifeSpan < 0)
        OnCollideWith(nullptr);
}

void Bullet::OnCollideWith(const mono::IBodyPtr& body)
{
    m_collisionCallback(this, body);
}

void Bullet::OnPostStep()
{ }
