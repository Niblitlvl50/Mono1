
#include "Bullet.h"

#include "Physics/CMIBody.h"
#include "Physics/CMIShape.h"
#include "Physics/CMFactory.h"

#include "IRenderer.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "Audio/ISound.h"
#include "Audio/AudioFactory.h"
#include "Utils.h"

using namespace game;

Bullet::Bullet(const BulletConfiguration& config, mono::EventHandler& event_handler)
    : m_eventHandler(event_handler),
      m_collisionCallback(config.collision_callback)
{
    mScale = math::Vector2f(25.0f, 25.0f);

    mPhysicsObject.body = mono::PhysicsFactory::CreateBody(1.0f, 1.0f);
    mPhysicsObject.body->SetCollisionHandler(this);

    mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(
        mPhysicsObject.body,
        config.collision_radius,
        math::Vector2f(0.0f, 0.0f)
    );

    mPhysicsObject.body->SetMoment(shape->GetInertiaValue());
    mPhysicsObject.shapes.push_back(shape);

    m_sprite = mono::CreateSprite(config.sprite_file);

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
    m_collisionCallback(this, body, m_eventHandler);
}

void Bullet::OnPostStep()
{ }
