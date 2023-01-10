
#pragma once

#include "EntitySystem/ObjectAttribute.h"

#include <cstdint>
#include <vector>
#include <string>

namespace mono
{
    struct ComponentData
    {
        uint32_t hash;
        std::string name;
        std::vector<Attribute> properties;
    };

    struct EntityData
    {
        std::string entity_name;
        uint32_t entity_uuid;
        uint32_t entity_properties;
        std::vector<ComponentData> entity_components;
    };
}
