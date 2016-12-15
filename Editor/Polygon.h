//
//  Polygon.h
//  MonoiOS
//
//  Created by Niklas Damberg on 25/06/16.
//
//

#pragma once

#include "Math/Vector.h"
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

        void AddVertex(const math::Vector& vertex);
        void SetVertex(const math::Vector& vertex, size_t index);
        const std::vector<math::Vector>& GetVertices() const;

        void SetSelected(bool selected);
        bool IsSelected() const;

        void SetTextureRepeate(float repeate);
        float GetTextureRepate() const;

        void SetTexture(const char* texture);
        const char* GetTexture() const;

    private:

        math::Quad LocalBoundingBox() const;

        void RecalculateTextureCoordinates();

        bool m_selected;
        mono::ITexturePtr m_texture;

        std::vector<math::Vector> m_points;
        std::vector<math::Vector> m_textureCoordinates;
        float m_texture_repeate;
        char m_texture_name[32];
    };

    struct Polygon
    {
        std::vector<math::Vector> vertices;
        std::vector<math::Vector> texture_coordinates;
        math::Vector centroid;
        float texture_repeate;
    };
}
