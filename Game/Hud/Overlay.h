
#pragma once

#include "IDrawable.h"

namespace game
{
    class UIOverlayDrawer : public mono::IDrawable
    {
    public:

        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual math::Quad BoundingBox() const;
    };
}
