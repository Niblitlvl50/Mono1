
#pragma once

#include "Rendering/IDrawable.h"

namespace mono
{
    class TransformSystemDrawer : public mono::IDrawable
    {
    public:
        TransformSystemDrawer(const class TransformSystem* transform_system);
        void doDraw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

    private:
        const class TransformSystem* m_transform_system;
    };
}
