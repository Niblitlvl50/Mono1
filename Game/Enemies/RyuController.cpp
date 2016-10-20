
#include "RyuController.h"
#include "Sprite/ISprite.h"
#include "EventHandler/EventHandler.h"
#include "Events/EventFuncFwd.h"
#include "Events/KeyDownEvent.h"
#include "System/SysKeycodes.h"

using namespace game;

namespace
{
    enum Animation
    {
        IDLE,
        RUNNING,
        JUMPING,
        FIGHTING
    };
}

RyuController::RyuController(mono::EventHandler& event_handler)
    : m_eventHandler(event_handler)
{
    using namespace std::placeholders;
    
    const event::KeyDownEventFunc& key_down_func = std::bind(&RyuController::OnKeyDown, this, _1);
    m_keyDownToken = m_eventHandler.AddListener(key_down_func);
}

RyuController::~RyuController()
{
    m_eventHandler.RemoveListener(m_keyDownToken);
}

bool RyuController::OnKeyDown(const event::KeyDownEvent& event)
{
    if(event.key == Key::RIGHT)
        m_target.x += 20.0f;
    else if(event.key == Key::LEFT)
        m_target.x -= 20.0f;
    
    if(event.key == Key::UP)
        m_target.y += 20.0f;
    else if(event.key == Key::DOWN)
        m_target.y -= 20.0f;

    return true;
}

void RyuController::Initialize(Enemy* enemy)
{
    m_enemy = enemy;
    m_target = enemy->Position();
}

void RyuController::doUpdate(unsigned int delta)
{
    constexpr float SPEED = 0.1f;
    const float value = (delta * SPEED);

    math::Vector2f position = m_enemy->Position();

    const bool xzero = (std::floor(std::abs(position.x - m_target.x)) == 0.0f);
    const bool yzero = (std::floor(std::abs(position.y - m_target.y)) == 0.0f);
        
    if(!xzero)
    {
        if(position.x > m_target.x)
            position.x -= value;
        else if(position.x < m_target.x)
            position.x += value;
    }
    
    if(!yzero)
    {
        if(position.y > m_target.y)
            position.y -= value;
        else if(position.y < m_target.y)
            position.y += value;
    }

    m_enemy->SetPosition(position);
    
    if(xzero && yzero)
        m_enemy->m_sprite->SetAnimation(Animation::IDLE);
    else
        m_enemy->m_sprite->SetAnimation(Animation::RUNNING);
}
