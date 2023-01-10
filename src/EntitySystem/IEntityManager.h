
#pragma once

#include "MonoFwd.h"
#include "EntitySystem/Entity.h"
#include "EntitySystem/ObjectAttribute.h"

#include <cstdint>
#include <vector>
#include <functional>

namespace mono
{
    using ComponentCreateFunc = bool(*)(mono::Entity* entity, mono::SystemContext* context);
    using ComponentReleaseFunc = bool(*)(mono::Entity* entity, mono::SystemContext* context);
    using ComponentUpdateFunc = bool(*)(mono::Entity* entity, const std::vector<Attribute>& properties, mono::SystemContext* context);

    using ReleaseCallback = std::function<void (uint32_t entity_id)>;

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
        virtual mono::Entity CreateEntity(const char* name, uint32_t uuid_hash, const std::vector<uint32_t>& components) = 0;
        virtual mono::Entity CreateEntity(const char* entity_file) = 0;

        virtual std::vector<mono::Entity> CreateEntityCollection(const char* entity_file) = 0;

        virtual const mono::Entity* GetEntity(uint32_t id) const = 0;

        virtual bool AddComponent(uint32_t entity_id, uint32_t component_hash) = 0;
        virtual bool RemoveComponent(uint32_t entity_id, uint32_t component_hash) = 0;
        virtual bool SetComponentData(uint32_t entity_id, uint32_t component_hash, const std::vector<Attribute>& properties) = 0;

        virtual void RegisterComponent(
            uint32_t component_hash,
            ComponentCreateFunc create_component,
            ComponentReleaseFunc release_component,
            ComponentUpdateFunc update_component) = 0;

        virtual void SetEntityProperties(uint32_t entity_id, uint32_t properties) = 0;
        virtual uint32_t GetEntityProperties(uint32_t entity_id) const = 0;

        virtual void SetEntityName(uint32_t entity_id, const char* name) = 0;
        virtual const char* GetEntityName(uint32_t entity_id) const = 0;

        virtual uint32_t GetEntityUuid(uint32_t entity_id) const = 0;
        virtual uint32_t GetEntityIdFromUuid(uint32_t uuid) const = 0;

        virtual void ReleaseEntity(uint32_t entity_id) = 0;

        virtual void PushEntityStackRecord(const char* debug_name) = 0;
        virtual void PopEntityStackRecord() = 0;

        virtual uint32_t AddReleaseCallback(uint32_t entity_id, const ReleaseCallback& callback) = 0;
        virtual void RemoveReleaseCallback(uint32_t entity_id, uint32_t callback_id) = 0;

        virtual const std::vector<SpawnEvent>& GetSpawnEvents() const = 0;
    };
}
