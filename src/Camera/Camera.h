
#pragma once

#include "ICamera.h"
#include "Math/Quad.h"

namespace mono
{
    class Camera : public mono::ICamera
    {
    public:

        Camera();

        void doUpdate(const UpdateContext& update_context) override;

        void SetViewport(const math::Quad& viewport) override;
        void SetTargetViewport(const math::Quad& target) override;
        math::Quad GetViewport() const override;
        void SetPosition(const math::Vector& position) override;
        math::Vector GetPosition() const override;
        void SetWindowSize(const math::Vector& window_size);
        const math::Vector& GetWindowSize() const override;
        math::Vector ScreenToWorld(const math::Vector& screen_pos) const override;

    private:

        math::Vector m_window_size;
        math::Quad m_viewport;
        math::Quad m_target_viewport;
    };
}
