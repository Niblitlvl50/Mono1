
#pragma once

#include "Rendering/RenderPtrFwd.h"
#include "IUpdatable.h"

namespace game
{
    class CameraViewportReporter : public mono::IUpdatable
    {
    public:
        CameraViewportReporter(const mono::ICameraPtr& camera);
        void doUpdate(unsigned int delta) override;

    private:
        const mono::ICameraPtr& m_camera;
    };
}
