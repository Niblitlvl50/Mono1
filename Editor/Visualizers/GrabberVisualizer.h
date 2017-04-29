
#pragma once

#include "Rendering/IDrawable.h"
#include <vector>

namespace editor
{
    struct Grabber;

    class GrabberVisualizer : public mono::IDrawable
    {
    public:

        GrabberVisualizer(const std::vector<editor::Grabber>& grabbers);

        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual math::Quad BoundingBox() const;

        const std::vector<editor::Grabber>& m_grabbers;
    };
}
