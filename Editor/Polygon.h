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
    class PolygonEntity : public mono::EntityBase
    {
    public:

        PolygonEntity();

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        virtual math::Quad BoundingBox() const;

        void AddVertex(const math::Vector2f& vertex);
        const std::vector<math::Vector2f>& GetVertices() const;

        void SetSelected(bool selected);

    private:

        math::Quad LocalBoundingBox() const;

        bool m_selected;
        std::vector<math::Vector2f> m_points;
        math::Vector2f m_centroid;
    };
}
