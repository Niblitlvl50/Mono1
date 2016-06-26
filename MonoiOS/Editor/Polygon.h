//
//  Polygon.h
//  MonoiOS
//
//  Created by Niklas Damberg on 25/06/16.
//
//

#pragma once

#include "Vector2f.h"
#include "EntityBase.h"

#include <vector>

namespace editor
{
    struct Polygon
    {
        std::vector<math::Vector2f> points;
    };

    class PolygonEntity : public mono::EntityBase
    {
    public:

        PolygonEntity();

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        virtual math::Quad BoundingBox() const;

        bool m_selected;
        std::vector<math::Vector2f> m_points;
    };
}
