
#pragma once

#include "IEnemyFactory.h"

namespace game
{
    class EnemyFactory : public IEnemyFactory
    {
    public:

        EnemyFactory(mono::EventHandler& event_handler);
    
        EnemyPtr CreateCacoDemon(const math::Vector& position) override;
        EnemyPtr CreateRyu(const math::Vector& position) override;
        EnemyPtr CreateInvader(const math::Vector& position) override;
        EnemyPtr CreatePathInvader(const mono::IPathPtr& path) override;
        EnemyPtr CreateBlackSquare(const math::Vector& position) override;

    private:

        mono::EventHandler& m_eventHandler;
    };
}
