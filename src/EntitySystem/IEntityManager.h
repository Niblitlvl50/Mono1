
#pragma once

#include "MonoFwd.h"
#include "EntitySystem/Entity.h"
#include "EntitySystem/ObjectAttribute.h"

#include <cstdint>
#include <vector>
#include <functional>

#define ENUM_BIT(n) (1 << (n))

namespace mono
{
    using ComponentCreateFunc = bool(*)(mono::Entity* entity, mono::SystemContext* context);
    using ComponentReleaseFunc = bool(*)(mono::Entity* entity, mono::SystemContext* context);
    using ComponentUpdateFunc = bool(*)(mono::Entity* entity, const std::vector<Attribute>& properties, mono::SystemContext* context);
    using ComponentEnableFunc = bool(*)(mono::Entity* entity, bool enable, mono::SystemContext* context);

    // Use PreRelease if you want to do something with the object that is going to be released, this is before its actually released.
    enum ReleasePhase : int
    {
        PRE_RELEASE = ENUM_BIT(0),
        POST_RELEASE = ENUM_BIT(1),
        PRE_POST_RELEASE = PRE_RELEASE | POST_RELEASE
    };

    using ReleaseCallback = std::function<void (uint32_t entity_id, mono::ReleasePhase phase)>;

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

        virtual mono::Entity SpawnEntity(const char* entity_file) = 0;
        virtual std::vector<mono::Entity> SpawnEntityCollection(const char* entity_file) = 0;

        virtual const mono::Entity* GetEntity(uint32_t id) const = 0;

        virtual bool AddComponent(uint32_t entity_id, uint32_t component_hash) = 0;
        virtual bool RemoveComponent(uint32_t entity_id, uint32_t component_hash) = 0;
        virtual bool SetComponentData(uint32_t entity_id, uint32_t component_hash, const std::vector<Attribute>& properties) = 0;

        virtual void RegisterComponent(
            uint32_t component_hash,
            ComponentCreateFunc create_component,
            ComponentReleaseFunc release_component,
            ComponentUpdateFunc update_component,
            ComponentEnableFunc enable_component = nullptr) = 0;

        virtual void SetEntityProperties(uint32_t entity_id, uint32_t properties) = 0;
        virtual uint32_t GetEntityProperties(uint32_t entity_id) const = 0;

        virtual void SetEntityName(uint32_t entity_id, const char* name) = 0;
        virtual const char* GetEntityName(uint32_t entity_id) const = 0;

        virtual void SetEntityTags(uint32_t entity_id, const std::vector<uint32_t>& tags) = 0;
        virtual bool HasEntityTag(uint32_t entity_id, uint32_t tag) const = 0;
        virtual std::vector<uint32_t> CollectEntitiesWithTag(uint32_t tag) const = 0;

        virtual void SetEntityEnabled(uint32_t entity_id, bool enable) = 0;

        virtual uint32_t GetEntityUuid(uint32_t entity_id) const = 0;
        virtual uint32_t GetEntityIdFromUuid(uint32_t uuid) const = 0;

        virtual void ReleaseEntity(uint32_t entity_id) = 0;
        virtual void ReleaseEntities(const std::vector<mono::Entity>& entities) = 0;

        virtual void PushEntityStackRecord(const char* debug_name) = 0;
        virtual void PopEntityStackRecord() = 0;

        virtual void SetLifetimeDependency(uint32_t entity_id, uint32_t dependency_entity_id) = 0;

        virtual uint32_t AddReleaseCallback(uint32_t entity_id, uint32_t callback_phases, const ReleaseCallback& callback) = 0;
        virtual void RemoveReleaseCallback(uint32_t entity_id, uint32_t callback_id) = 0;

        virtual const std::vector<SpawnEvent>& GetSpawnEvents() const = 0;
    };
}
