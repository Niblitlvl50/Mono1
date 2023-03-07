
#pragma once

#include "Rendering/IDrawable.h"

namespace mono
{
    class ScreenFadeDrawer : public mono::IDrawable
    {
    public:

        ScreenFadeDrawer(const class RenderSystem* render_system);
        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        const class RenderSystem* m_render_system;
    };
}
