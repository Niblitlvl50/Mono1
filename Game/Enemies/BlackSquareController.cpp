
#include "BlackSquareController.h"
#include "EntityProperties.h"
#include "AIKnowledge.h"
#include "Events/SpawnConstraintEvent.h"

#include "Physics/CMFactory.h"
#include "Physics/IBody.h"
#include "Physics/IConstraint.h"
#include "Physics/ConstraintsFactory.h"
#include "EventHandler/EventHandler.h"
#include "Rendering/Sprite/ISprite.h"
#include "Rendering/Color.h"

using namespace game;


BlackSquareController::BlackSquareController(float trigger_distance, mono::EventHandler& event_handler)
    : m_triggerDistance(trigger_distance),
      m_eventHandler(event_handler),
      m_awakeStateTimer(0)
{ }

BlackSquareController::~BlackSquareController()
{
    m_eventHandler.DispatchEvent(DespawnConstraintEvent(m_spring));
}

void BlackSquareController::Initialize(Enemy* enemy)
{
    m_enemy = enemy;
    m_enemy->SetProperty(EntityProperties::DAMAGABLE);

    m_controlBody = mono::PhysicsFactory::CreateKinematicBody();
    m_controlBody->SetPosition(m_enemy->Position());

    m_spring = mono::ConstraintsFactory::CreateSpring(m_controlBody, m_enemy->GetPhysics().body, 1.0f, 20.0f, 0.5f);

    m_eventHandler.DispatchEvent(SpawnConstraintEvent(m_spring));

    TransitionToState(State::SLEEPING);
}

void BlackSquareController::doUpdate(unsigned int delta)
{
    switch(m_state)
    {
        case State::SLEEPING:
            SleepState(delta);
            break;
        case State::AWAKE:
            AwakeState(delta);
            break;
        case State::HUNT:
            HuntState(delta);
            break;
    }
}

void BlackSquareController::TransitionToState(State new_state)
{
    switch(new_state)
    {
        case State::SLEEPING:
            m_enemy->m_sprite->SetShade(mono::Color::RGBA(0.0f, 0.0f, 0.0f, 1.0f));
            m_enemy->GetPhysics().body->ResetForces();
            m_controlBody->SetPosition(m_enemy->Position());
            break;
        case State::AWAKE:
            m_enemy->m_sprite->SetShade(mono::Color::RGBA(1.0f, 1.0f, 1.0f, 1.0f));
            m_awakeStateTimer = 0;
            break;
        case State::HUNT:
            m_enemy->m_sprite->SetShade(mono::Color::RGBA(1.0f, 0.0f, 0.0f, 1.0f));
            break;
    }

    m_state = new_state;
}

void BlackSquareController::SleepState(unsigned int delta)
{
    const float distance = math::Length(player_position - m_enemy->Position());
    if(distance < m_triggerDistance)
        TransitionToState(State::AWAKE);
}

void BlackSquareController::AwakeState(unsigned int delta)
{
    m_awakeStateTimer += delta;
    if(m_awakeStateTimer > 500)
        TransitionToState(State::HUNT);
}

void BlackSquareController::HuntState(unsigned int delta)
{
    m_controlBody->SetPosition(player_position);

    const float distance = math::Length(player_position - m_enemy->Position());
    if(distance > m_triggerDistance)
        TransitionToState(State::SLEEPING);
}
