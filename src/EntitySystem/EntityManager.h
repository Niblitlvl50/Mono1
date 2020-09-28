
#pragma once

#include "IEntityManager.h"

#include "MonoFwd.h"
#include "EntitySystem/EntitySystem.h"
#include "ObjectAttribute.h"

#include <string>
#include <vector>
#include <unordered_set>
#include <unordered_map>

using ComponentCreateFunc = bool(*)(mono::Entity* entity, mono::SystemContext* context);
using ComponentReleaseFunc = bool(*)(mono::Entity* entity, mono::SystemContext* context);
using ComponentUpdateFunc = bool(*)(mono::Entity* entity, const std::vector<Attribute>& properties, mono::SystemContext* context);
using ComponentGetFunc = std::vector<Attribute>(*)(const mono::Entity* entity, mono::SystemContext* context);

namespace mono
{
    class EntityManager : public mono::IEntityManager
    {
    public:

        EntityManager(mono::EntitySystem* entity_system, mono::SystemContext* system_context);
        ~EntityManager();

        mono::Entity CreateEntity(const char* name, const std::vector<uint32_t>& components) override;
        mono::Entity CreateEntity(const char* entity_file) override;

        bool AddComponent(uint32_t entity_id, uint32_t component_hash) override;
        bool RemoveComponent(uint32_t entity_id, uint32_t component_hash) override;
        bool SetComponentData(uint32_t entity_id, uint32_t component_hash, const std::vector<Attribute>& properties) override;
        std::vector<Attribute> GetComponentData(uint32_t entity_id, uint32_t component_hash) const override;

        void SetEntityProperties(uint32_t entity_id, uint32_t properties) override;
        uint32_t GetEntityProperties(uint32_t entity_id) const override;

        void ReleaseEntity(uint32_t entity_id) override;
        const std::vector<SpawnEvent>& GetSpawnEvents() const override;
        void Sync() override;

        void RegisterComponent(
            uint32_t component_hash,
            ComponentCreateFunc create_component,
            ComponentReleaseFunc release_component,
            ComponentUpdateFunc update_component,
            ComponentGetFunc get_component = nullptr);

    private:

        void DeferredRelease();

        mono::EntitySystem* m_entity_system;
        mono::SystemContext* m_system_context;
        std::unordered_set<uint32_t> m_entities_to_release;
        std::unordered_map<uint32_t, EntityData> m_cached_entities;

        struct ComponentFuncs
        {
            ComponentCreateFunc create;
            ComponentReleaseFunc release;
            ComponentUpdateFunc update;
            ComponentGetFunc get;
        };

        std::unordered_map<uint32_t, ComponentFuncs> m_component_factories;
        std::vector<SpawnEvent> m_spawn_events;
    };
}
