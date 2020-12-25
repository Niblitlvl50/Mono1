
#pragma once

#include "EntitySystem/Entity.h"
#include "EntitySystem/ObjectAttribute.h"

#include <cstdint>
#include <vector>
#include <string>

namespace mono
{
    struct ComponentData
    {
        std::string name;
        std::vector<Attribute> properties;
    };

    struct EntityData
    {
        std::string entity_name;
        uint32_t entity_properties;
        std::vector<ComponentData> entity_components;
    };

    class IEntityManager
    {
    public:

        struct SpawnEvent
        {
            bool spawned;
            uint32_t entity_id;
        };

        virtual ~IEntityManager() = default;
        virtual mono::Entity CreateEntity(const char* name, const std::vector<uint32_t>& components) = 0;
        virtual mono::Entity CreateEntity(const char* entity_file) = 0;

        virtual bool AddComponent(uint32_t entity_id, uint32_t component_hash) = 0;
        virtual bool RemoveComponent(uint32_t entity_id, uint32_t component_hash) = 0;
        virtual bool SetComponentData(uint32_t entity_id, uint32_t component_hash, const std::vector<Attribute>& properties) = 0;
        virtual std::vector<Attribute> GetComponentData(uint32_t entity_id, uint32_t component_hash) const = 0;

        virtual void SetEntityProperties(uint32_t entity_id, uint32_t properties) = 0;
        virtual uint32_t GetEntityProperties(uint32_t entity_id) const = 0;

        virtual void ReleaseEntity(uint32_t entity_id) = 0;
        virtual void ReleaseAllEntities() = 0;

        virtual const std::vector<SpawnEvent>& GetSpawnEvents() const = 0;
        virtual void Sync() = 0;

        // Game implementation needed...
        // ...this could probably be solved with function pointers passed in.
        virtual EntityData LoadEntityFile(const char* entity_file) const = 0;
        virtual const char* ComponentNameFromHash(uint32_t component_hash) const = 0;
    };
}
