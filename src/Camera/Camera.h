
#pragma once

#include "ICamera.h"
#include "Math/Quad.h"

namespace mono
{
    class Camera : public mono::ICamera
    {
    public:

        Camera();

        void Update(const UpdateContext& update_context) override;

        void SetViewportSize(const math::Vector& size) override;
        void SetTargetViewportSize(const math::Vector& target_size) override;
        math::Vector GetViewportSize() const override;
        math::Quad GetViewport() const override;

        void SetPosition(const math::Vector& position) override;
        void SetTargetPosition(const math::Vector& target_position) override;
        math::Vector GetPosition() const override;
        math::Vector GetTargetPosition() const override;

        void SetPositionOffset(const math::Vector& offset) override;

        void SetWindowSize(const math::Vector& window_size) override;
        const math::Vector& GetWindowSize() const override;
        math::Vector ScreenToWorld(const math::Vector& screen_pos) const override;

    private:

        math::Vector m_position;
        math::Vector m_target_position;
        math::Vector m_position_offset;

        math::Vector m_viewport_size;
        math::Vector m_target_viewport_size;

        math::Vector m_window_size;
    };
}
