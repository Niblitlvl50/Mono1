
#pragma once

#include "ICamera.h"
#include <vector>

namespace mono
{
    class CameraSwitch : public mono::ICamera
    {
    public:

        CameraSwitch(const std::vector<ICameraPtr>& cameras);
        void SetActiveCamera(int index);

    private:

        void doUpdate(unsigned int delta) override;
        
        void Follow(uint32_t entity_id, const math::Vector& offset) override;
        void Unfollow() override;
        math::Quad GetViewport() const override;
        math::Vector GetPosition() const override;
        void SetViewport(const math::Quad& viewport) override;
        void SetTargetViewport(const math::Quad& target) override;
        void SetPosition(const math::Vector& position) override;
        math::Vector ScreenToWorld(const math::Vector& screen_pos, const math::Vector& window_size) const override;

        const std::vector<ICameraPtr> m_cameras;
        ICameraPtr m_active_camera;
    };
}
