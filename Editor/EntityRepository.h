
#pragma once

#include "Math/Vector.h"

#include <string>
#include <vector>

namespace editor
{
    struct EntityDefinition
    {
        std::string name;
        std::string sprite_file;
        math::Vector scale;
    };

    struct SnapPoint
    {
        math::Vector position;
        float normal = 0;
    };

    struct PrefabDefinition
    {
        std::string name;
        std::string sprite_file;
        std::vector<math::Vector> collision_shape;
        std::vector<SnapPoint> snap_points;
    };

    class EntityRepository
    {
    public:

        bool LoadDefinitions();

        const EntityDefinition& GetDefinitionFromName(const std::string& name) const;
        const PrefabDefinition& GetPrefabFromName(const std::string& name) const;

        std::vector<EntityDefinition> m_entities;
        std::vector<PrefabDefinition> m_prefabs;
    };
}
