
#include "InvaderController.h"
#include "EntityProperties.h"
#include "Events/SpawnConstraintEvent.h"

#include "EventHandler/EventHandler.h"
#include "Sprite/ISprite.h"
#include "Physics/CMFactory.h"
#include "Physics/IBody.h"
#include "Physics/IConstraint.h"
#include "Physics/ConstraintsFactory.h"
#include "Paths/IPath.h"


#include "Events/SpawnEntityEvent.h"
#include "EntityBase.h"
#include "IRenderer.h"
#include "Color.h"

namespace
{
    class DotEntity : public mono::EntityBase
    {
    public:

        DotEntity(const math::Vector& point)
            : m_point(point)
        {
            mScale = math::Vector(10, 10);
        }

        virtual void Draw(mono::IRenderer& renderer) const
        {
            constexpr mono::Color::RGBA color(1.0f, 0.5f, 1.0f, 1.0f);
            renderer.DrawPoints({ math::zeroVec }, color, 10.0f);
        }

        virtual void Update(unsigned int delta)
        {
            mPosition = m_point;
        }
        
        const math::Vector& m_point;
    };
}

using namespace game;

InvaderController::InvaderController(mono::EventHandler& event_handler)
{
    (void)event_handler;
}

void InvaderController::Initialize(Enemy* enemy)
{
    m_enemy = enemy;
    m_enemy->SetProperty(EntityProperties::DAMAGABLE);
}

void InvaderController::doUpdate(unsigned int delta)
{
    m_color.hue += (delta * 0.0005f);
    if(m_color.hue > 1.0f)
        m_color.hue = 0.0f;

    m_enemy->m_sprite->SetShade(mono::Color::ToRGBA(m_color));
}


InvaderPathController::InvaderPathController(const mono::IPathPtr& path, mono::EventHandler& event_handler)
    : m_path(path),
      m_eventHandler(event_handler),
      m_currentPosition(0.0f)
{ }

InvaderPathController::~InvaderPathController()
{
    m_eventHandler.DispatchEvent(DespawnConstraintEvent(m_spring));
}

void InvaderPathController::Initialize(Enemy* enemy)
{
    m_enemy = enemy;

    m_controlBody = mono::PhysicsFactory::CreateKinematicBody();
    m_spring = mono::ConstraintsFactory::CreateSpring(m_controlBody, m_enemy->GetPhysics().body, 5.0f, 3.0f, 0.3f);

    m_eventHandler.DispatchEvent(SpawnConstraintEvent(m_spring));
    m_eventHandler.DispatchEvent(SpawnEntityEvent(std::make_shared<DotEntity>(m_point)));
}

void InvaderPathController::doUpdate(unsigned int delta)
{
    constexpr float speed_mps = 50.0f;
    const float position_add = speed_mps * float(delta) / 1000.0f;
    m_currentPosition += position_add;

    const math::Vector& position = m_path->GetPositionByLength(m_currentPosition) + m_path->GetGlobalPosition();
    m_point = position;
    m_controlBody->SetPosition(position);

    if(m_currentPosition > m_path->Length())
        m_currentPosition = 0.0f;
}
