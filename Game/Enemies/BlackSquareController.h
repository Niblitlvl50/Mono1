
#pragma once

#include "MonoPtrFwd.h"
#include "Enemy.h"

namespace game
{
    class BlackSquareController : public IEnemyController
    {
    public:

        BlackSquareController(float trigger_distance, mono::EventHandler& event_handler);
        virtual ~BlackSquareController();

        virtual void Initialize(Enemy* enemy);
        virtual void doUpdate(unsigned int delta);

    private:

        enum class State
        {
            SLEEPING,
            AWAKE,
            HUNT
        };

        void TransitionToState(State new_state);
        void SleepState(unsigned int delta);
        void AwakeState(unsigned int delta);
        void HuntState(unsigned int delta);

        const float m_triggerDistance;
        mono::EventHandler& m_eventHandler;

        Enemy* m_enemy;
        State m_state;
        unsigned int m_awakeStateTimer;

        mono::IBodyPtr m_controlBody;
        mono::IConstraintPtr m_spring;
    };
}
