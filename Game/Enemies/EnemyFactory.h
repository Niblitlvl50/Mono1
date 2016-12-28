
#pragma once

#include "MonoFwd.h"
#include "MonoPtrFwd.h"
#include "Math/MathFwd.h"
#include <memory>

namespace game
{
    class Enemy;
    using EnemyPtr = std::shared_ptr<Enemy>;

    std::shared_ptr<Enemy> CreateCacoDemon(const math::Vector& position, mono::EventHandler& event_handler);
    std::shared_ptr<Enemy> CreateRyu(const math::Vector& position, mono::EventHandler& event_handler);
    std::shared_ptr<Enemy> CreateInvader(const math::Vector& position, mono::EventHandler& event_handler);

    EnemyPtr CreatePathInvader(const mono::IPathPtr& path, mono::EventHandler& event_handler);
}
