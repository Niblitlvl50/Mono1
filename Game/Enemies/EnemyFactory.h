
#pragma once

#include "MonoFwd.h"
#include "Math/MathFwd.h"
#include <memory>

namespace game
{
    class Enemy;

    std::shared_ptr<Enemy> CreateCacoDemon(const math::Vector2f& position, mono::EventHandler& event_handler);
    std::shared_ptr<Enemy> CreateRyu(const math::Vector2f& position, mono::EventHandler& event_handler);
}
