
#pragma once

#include "MonoFwd.h"
#include "Rendering/IDrawable.h"

namespace mono
{
    class LightSystemDrawer : public mono::IDrawable
    {
    public:

        LightSystemDrawer(const LightSystem* light_system, const mono::TransformSystem* transform_system);

        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        const LightSystem* m_light_system;
        const TransformSystem* m_transform_system;
    };
}
