
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

    class EntityRepository
    {
    public:

        EntityRepository();

        bool LoadDefinitions();
        const EntityDefinition& GetDefinitionFromName(const std::string& name) const;

        std::vector<EntityDefinition> m_entities;
    };
}
