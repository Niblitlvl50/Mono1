
#pragma once

#include "Math/MathFwd.h"
#include "IUpdatable.h"

namespace mono
{
    class ICamera : public IUpdatable
    {
    public:

        virtual void SetViewport(const math::Quad& viewport) = 0;
        virtual void SetTargetViewport(const math::Quad& target) = 0;
        virtual math::Quad GetViewport() const = 0;

        virtual void SetPosition(const math::Vector& position) = 0;
        virtual math::Vector GetPosition() const = 0;

        virtual math::Vector ScreenToWorld(const math::Vector& screen_pos, const math::Vector& window_size) const = 0;
    };
}
