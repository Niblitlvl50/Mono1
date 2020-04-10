
#pragma once

#include "ICamera.h"
#include "Math/Quad.h"

namespace mono
{
    class Camera : public mono::ICamera
    {
    public:

        Camera();

        virtual void doUpdate(const UpdateContext& update_context);

        virtual void SetViewport(const math::Quad& viewport);
        virtual void SetTargetViewport(const math::Quad& target);
        virtual math::Quad GetViewport() const;

        virtual void SetPosition(const math::Vector& position);
        virtual math::Vector GetPosition() const;

        virtual math::Vector ScreenToWorld(const math::Vector& screen_pos, const math::Vector& window_size) const;

    private:

        math::Quad m_viewport;
        math::Quad m_target_viewport;
    };
}
