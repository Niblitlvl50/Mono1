
#pragma once

#include "MonoFwd.h"
#include "MonoPtrFwd.h"
#include "Math/MathFwd.h"
#include <memory>

namespace game
{
    class Enemy;
    using EnemyPtr = std::shared_ptr<Enemy>;

    class IEnemyFactory
    {
    public:
        virtual ~IEnemyFactory()
        { }

        virtual EnemyPtr CreateCacoDemon(const math::Vector& position) = 0;
        virtual EnemyPtr CreateRyu(const math::Vector& position) = 0;
        virtual EnemyPtr CreateInvader(const math::Vector& position) = 0;
        virtual EnemyPtr CreatePathInvader(const mono::IPathPtr& path) = 0;
        virtual EnemyPtr CreateBlackSquare(const math::Vector& position) = 0;
    };
}
