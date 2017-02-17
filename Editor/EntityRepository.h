
#pragma once

#include <string>
#include <vector>

namespace editor
{
    struct EntityDefinition
    {
        std::string name;
        std::string sprite_file;
    };

    class EntityRepository
    {
    public:

        EntityRepository();
        bool LoadDefinitions();

        std::vector<EntityDefinition> m_entities;
    };
}
