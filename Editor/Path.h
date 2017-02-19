
#pragma once

#include "EntityBase.h"
#include "Math/Vector.h"
#include <vector>
#include <string>

namespace editor
{
    class PathEntity : public mono::EntityBase
    {
    public:

        PathEntity(const std::string& name);
        PathEntity(const std::string& name, const std::vector<math::Vector>& points);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        virtual math::Quad BoundingBox() const;

        void SetSelected(bool selected);

        void AddVertex(const math::Vector& vertex);
        void SetVertex(const math::Vector& vertex, size_t index);

        void SetName(const char* new_name);

        std::string m_name;
        std::vector<math::Vector> m_points;
        bool m_selected;
    };
}
