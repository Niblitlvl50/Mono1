
#pragma once

#include "MonoFwd.h"
#include "Rendering/IDrawable.h"

namespace mono
{
    class TriggerDebugDrawer : public mono::IDrawable
    {
    public:

        TriggerDebugDrawer(const bool& enabled, mono::TriggerSystem* trigger_system, const mono::TransformSystem* transform_system);
        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

    private:

        const bool& m_enabled;
        mono::TriggerSystem* m_trigger_system;
        const mono::TransformSystem* m_transform_system;
    };
}
