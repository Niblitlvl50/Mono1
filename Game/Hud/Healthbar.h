
#pragma once

#include "Math/Vector.h"
#include "Rendering/IDrawable.h"
#include <vector>

namespace game
{
    struct Healthbar
    {
        math::Vector position;
        float health_percentage;
        float width;
    };

    class HealthbarDrawer : public mono::IDrawable
    {
    public:
        HealthbarDrawer(const std::vector<Healthbar>& healthbars);

        virtual void doDraw(mono::IRenderer& renderer) const;
        virtual math::Quad BoundingBox() const;

        const std::vector<Healthbar>& m_healthbars;
    };
}
