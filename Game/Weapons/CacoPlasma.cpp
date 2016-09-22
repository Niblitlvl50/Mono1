//
//  CacoPlasma.cpp
//  MonoOSX
//
//  Created by Niklas Damberg on 23/12/15.
//
//

#include "CacoPlasma.h"
#include "Math/MathFunctions.h"
#include "Math/Vector2f.h"

#include "EntityBase.h"
#include "PhysicsEntityBase.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "IRenderer.h"
#include "EventHandler/EventHandler.h"
#include "Physics/CMIBody.h"
#include "Physics/CMIShape.h"
#include "Physics/CMFactory.h"

#include "Events/SpawnEntityEvent.h"
#include "Events/RemoveEntityEvent.h"
#include "Events/SpawnPhysicsEntityEvent.h"
#include "Events/DamageEvent.h"

#include "RenderLayers.h"

#include <cmath>

namespace
{
    class CacoExplosion : public mono::EntityBase
    {
    public:

        CacoExplosion(mono::EventHandler& event_handler, const math::Vector2f& position)
        {
            mPosition = position;
            mScale = math::Vector2f(40, 40);

            const uint id = Id();

            const auto func = [&event_handler, id]() {
                event_handler.DispatchEvent(game::RemoveEntityEvent(id));
            };

            m_sprite = mono::CreateSprite("sprites/cacoexplosion.sprite");
            m_sprite->SetAnimation(0, func);
        }

        virtual void Draw(mono::IRenderer& renderer) const
        {
            renderer.DrawSprite(*m_sprite);
        }

        virtual void Update(unsigned int delta)
        {
            m_sprite->doUpdate(delta);
        }

        mono::ISpritePtr m_sprite;
    };

    class CacoBullet : public mono::PhysicsEntityBase, public mono::ICollisionHandler
    {
    public:

        CacoBullet(const math::Vector2f& position, mono::EventHandler& eventHandler)
            : m_eventHandler(eventHandler)
        {
            mScale = math::Vector2f(20.0f, 20.0f);

            mPhysicsObject.body = mono::PhysicsFactory::CreateBody(1.0f, 1.0f);
            mPhysicsObject.body->SetPosition(position);
            mPhysicsObject.body->SetCollisionHandler(this);

            mono::IShapePtr shape = mono::PhysicsFactory::CreateShape(mPhysicsObject.body, 7.0f, math::Vector2f(0.0f, 0.0f));

            mPhysicsObject.body->SetMoment(shape->GetInertiaValue());
            mPhysicsObject.shapes.push_back(shape);

            m_sprite = mono::CreateSprite("sprites/cacobullet.sprite");
        }

        virtual void Draw(mono::IRenderer& renderer) const
        {
            renderer.DrawSprite(*m_sprite);
        }

        virtual void Update(unsigned int delta)
        {
            m_sprite->doUpdate(delta);
        }

        virtual void OnCollideWith(const mono::IBodyPtr& body)
        {
            const game::SpawnEntityEvent event(std::make_shared<CacoExplosion>(m_eventHandler, mPosition));
            m_eventHandler.DispatchEvent(event);
            
            m_eventHandler.DispatchEvent(game::DamageEvent(body, 20));
        }

        virtual void OnPostStep()
        {
            printf("Remove with id: %u\n", Id());
            m_eventHandler.DispatchEvent(game::RemoveEntityEvent(Id()));
        }

        mono::EventHandler& m_eventHandler;
        mono::ISpritePtr m_sprite;
    };
}

using namespace game;

CacoPlasma::CacoPlasma(mono::EventHandler& eventHandler)
    : BaseWeapon(eventHandler)
{ }

void CacoPlasma::DoFire(const math::Vector2f& position, float direction) const
{
    const math::Vector2f unit(-std::sin(direction), std::cos(direction));
    const math::Vector2f& new_position = position + (unit * 60.0f);
    const math::Vector2f& impulse = unit * 300.0f;

    auto bullet = std::make_shared<CacoBullet>(new_position, m_eventHandler);
    bullet->GetPhysics().body->ApplyImpulse(impulse, new_position);

    m_eventHandler.DispatchEvent(game::SpawnPhysicsEntityEvent(bullet, BACKGROUND));
}

int CacoPlasma::RoundsPerSecond() const
{
    return 2;
}
