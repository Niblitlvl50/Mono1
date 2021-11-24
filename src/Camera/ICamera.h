
#pragma once

#include "Math/MathFwd.h"
#include "IUpdatable.h"

namespace mono
{
    class ICamera : public IUpdatable
    {
    public:

        virtual void SetViewportSize(const math::Vector& size) = 0;
        virtual void SetTargetViewportSize(const math::Vector& target_size) = 0;
        virtual math::Vector GetViewportSize() const = 0;
        virtual math::Vector GetTargetViewportSize() const = 0;
        virtual math::Quad GetViewport() const = 0;

        virtual void Move(const math::Vector& delta) = 0;
        virtual void SetPosition(const math::Vector& position) = 0;
        virtual void SetTargetPosition(const math::Vector& target_position) = 0;
        virtual math::Vector GetPosition() const = 0;
        virtual math::Vector GetTargetPosition() const = 0;

        virtual void SetPositionOffset(const math::Vector& offset) = 0;

        virtual void SetWindowSize(const math::Vector& window_size) = 0;
        virtual const math::Vector& GetWindowSize() const = 0;
        
        virtual math::Vector ScreenToWorld(const math::Vector& screen_pos) const = 0;
    };
}
