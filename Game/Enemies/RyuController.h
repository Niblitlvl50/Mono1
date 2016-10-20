
#pragma once

#include "Enemy.h"
#include "Events/EventFwd.h"
#include "EventHandler/EventToken.h"

namespace game
{
    class RyuController : public IEnemyController
    {
    public:

        RyuController(mono::EventHandler& event_handler);
        ~RyuController();

        bool OnKeyDown(const event::KeyDownEvent& event);

        virtual void Initialize(Enemy* enemy);
        virtual void doUpdate(unsigned int delta);

    private:

        mono::EventHandler& m_eventHandler;
        mono::EventToken<event::KeyDownEvent> m_keyDownToken;

        math::Vector2f m_target;
        Enemy* m_enemy;
    };
}
