
#pragma once

#include "Rendering/IDrawable.h"

namespace mono
{
    class AudioDebugDrawer : public mono::IDrawable
    {
    public:

        AudioDebugDrawer(const bool& enabled);

        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

    private:

        const bool& m_enabled;
    };
}
