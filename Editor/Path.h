
#pragma once

#include "EntityBase.h"
#include "Math/Vector2f.h"
#include <vector>
#include <string>

namespace editor
{
    class PathEntity : public mono::EntityBase
    {
    public:

        PathEntity(const std::string& name, const std::vector<math::Vector2f>& points);

        virtual void Draw(mono::IRenderer& renderer) const;
        virtual void Update(unsigned int delta);
        virtual math::Quad BoundingBox() const;

        std::string m_name;
        std::vector<math::Vector2f> m_points;
    };
}
