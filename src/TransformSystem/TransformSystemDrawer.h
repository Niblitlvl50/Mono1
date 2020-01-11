
#pragma once

#include "Rendering/IDrawable.h"

namespace mono
{
    class TransformSystemDrawer : public mono::IDrawable
    {
    public:
        TransformSystemDrawer(const bool& enabled, const class TransformSystem* transform_system);
        void doDraw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

    private:
        const bool& m_enabled;
        const class TransformSystem* m_transform_system;
    };
}
