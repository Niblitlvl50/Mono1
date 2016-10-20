
#pragma once

#include "Enemy.h"
#include "Color.h"

namespace game
{
    class InvaderController : public IEnemyController
    {
    public:

        InvaderController(mono::EventHandler& event_handler);
        virtual void Initialize(Enemy* enemy);
        virtual void doUpdate(unsigned int delta);

    private:

        Enemy* m_enemy;
        mono::Color::HSL m_color;
    };
}
