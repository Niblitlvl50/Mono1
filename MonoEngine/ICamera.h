
#pragma once

#include "MonoPtrFwd.h"
#include "Math/MathFwd.h"

namespace mono
{
    struct ICamera
    {
        virtual ~ICamera()
        { }

        virtual void Update(unsigned int delta) = 0;

        virtual void Follow(const mono::IEntityPtr& entity, const math::Vector& offset) = 0;
        virtual void Unfollow() = 0;

        virtual math::Quad GetViewport() const = 0;
        virtual math::Vector GetPosition() const = 0;

        virtual void SetViewport(const math::Quad& viewport) = 0;
        virtual void SetTargetViewport(const math::Quad& target) = 0;
        virtual void SetPosition(const math::Vector& position) = 0;

        virtual math::Vector ScreenToWorld(const math::Vector& screen_pos, const math::Vector& window_size) const = 0;
    };
}
