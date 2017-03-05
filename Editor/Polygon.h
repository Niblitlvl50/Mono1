
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
        PolygonEntity(const std::vector<math::Vector>& points);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        virtual math::Quad BoundingBox() const;

        void AddVertex(const math::Vector& vertex);
        void SetVertex(const math::Vector& vertex, size_t index);
        const std::vector<math::Vector>& GetVertices() const;

        void SetSelected(bool selected);
        bool IsSelected() const;

        void SetTexture(const char* texture);
        const char* GetTexture() const;

    private:

        math::Quad LocalBoundingBox() const;

        void RecalculateTextureCoordinates();

        bool m_selected;
        mono::ITexturePtr m_texture;

        std::vector<math::Vector> m_points;
        std::vector<math::Vector> m_textureCoordinates;
        char m_texture_name[32];
    };

    void DrawPolygon(mono::IRenderer& renderer,
                     const mono::ITexturePtr& texture,
                     const std::vector<math::Vector>& points,
                     const std::vector<math::Vector>& texture_coords);
}
