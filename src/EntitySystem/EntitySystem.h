
#pragma once

#include "MonoFwd.h"

#include "IEntityManager.h"
#include "IGameSystem.h"

#include "EntityTypes.h"
#include "ObjectAttribute.h"

#include <string>
#include <vector>
#include <array>
#include <unordered_set>
#include <unordered_map>

using ComponentNameLookupFunc = const char* (*)(uint32_t component_hash);
using AttributeNameLookupFunc = const char* (*)(uint32_t attribute_hash);

namespace mono
{
    class EntitySystem : public mono::IGameSystem, public mono::IEntityManager
    {
    public:

        EntitySystem(
            uint32_t n_entities,
            mono::SystemContext* system_context,
            ComponentNameLookupFunc component_lookup,
            AttributeNameLookupFunc attribute_lookup);
        ~EntitySystem();

        mono::Entity CreateEntity(const char* name, const std::vector<uint32_t>& components) override;
        mono::Entity CreateEntity(const char* name, uint32_t uuid_hash, const std::vector<uint32_t>& components) override;
        mono::Entity CreateEntity(const char* entity_file) override;

        std::vector<mono::Entity> CreateEntityCollection(const char* entity_file) override;

        bool AddComponent(uint32_t entity_id, uint32_t component_hash) override;
        bool RemoveComponent(uint32_t entity_id, uint32_t component_hash) override;
        bool SetComponentData(uint32_t entity_id, uint32_t component_hash, const std::vector<Attribute>& properties) override;

        void SetEntityProperties(uint32_t entity_id, uint32_t properties) override;
        uint32_t GetEntityProperties(uint32_t entity_id) const override;

        void SetEntityName(uint32_t entity_id, const char* name) override;
        const char* GetEntityName(uint32_t entity_id) const override;

        void SetEntityTags(uint32_t entity_id, const std::vector<uint32_t>& tags) override;
        bool HasEntityTag(uint32_t entity_id, uint32_t tag) const override;

        void SetEntityEnabled(uint32_t entity_id, bool enable) override;

        uint32_t GetEntityUuid(uint32_t entity_id) const override;
        uint32_t GetEntityIdFromUuid(uint32_t uuid) const override;

        void ReleaseEntity(uint32_t entity_id) override;
        void ReleaseEntities(const std::vector<mono::Entity>& entities) override;

        void PushEntityStackRecord(const char* debug_name) override;
        void PopEntityStackRecord() override;


        uint32_t AddReleaseCallback(uint32_t entity_id, const ReleaseCallback& callback) override;
        void RemoveReleaseCallback(uint32_t entity_id, uint32_t callback_id) override;

        const std::vector<SpawnEvent>& GetSpawnEvents() const override;
        void Sync() override;
        void Reset() override;
        void Destroy() override;

        void RegisterComponent(
            uint32_t component_hash,
            ComponentCreateFunc create_component,
            ComponentReleaseFunc release_component,
            ComponentUpdateFunc update_component,
            ComponentEnableFunc enable_component) override;

        Entity* AllocateEntity(const char* name);
        void ReleaseEntity2(uint32_t entity_id);
        Entity* GetEntity(uint32_t entity_id);
        const Entity* GetEntity(uint32_t entity_id) const override;

        void SetProperty(Entity entity, uint32_t property);
        bool HasProperty(Entity entity, uint32_t property) const;

        bool HasComponent(const mono::Entity* entity, uint32_t component_hash) const;

        void SetName(uint32_t entity_id, const std::string& name);
        const std::string& GetName(uint32_t entity_id) const;
        uint32_t FindEntityByName(const char* name) const;

        template <typename T>
        inline void ForEachEntity(T&& functor)
        {
            for(size_t index = 0; index < m_entities.size(); ++index)
            {
                Entity& entity = m_entities[index];
                if(entity.id != INVALID_ID)
                    functor(entity);
            }
        }

        std::vector<uint32_t> CollectEntitiesWithTag(uint32_t tag) const;

        const char* Name() const override;
        void Update(const UpdateContext& update_context) override;

        static const char* AttributeNameLookup(uint32_t attribute_type_hash);

    private:

        void DeferredRelease();

        mono::SystemContext* m_system_context;
        ComponentNameLookupFunc m_component_name_lookup;
        static AttributeNameLookupFunc s_attribute_name_lookup;

        std::vector<Entity> m_entities;
        std::vector<uint32_t> m_entity_uuids;
        std::vector<std::vector<uint32_t>> m_entity_tags;
        std::vector<uint32_t> m_free_indices;
        std::vector<std::string> m_debug_names;

        using ReleaseCallbacks = std::array<ReleaseCallback, 8>;
        std::vector<ReleaseCallbacks> m_release_callbacks;

        std::unordered_set<uint32_t> m_entities_to_release;
        std::unordered_map<uint32_t, std::vector<EntityData>> m_cached_entities;

        struct ComponentFuncs
        {
            ComponentCreateFunc create;
            ComponentReleaseFunc release;
            ComponentUpdateFunc update;
            ComponentEnableFunc enable;
        };

        std::unordered_map<uint32_t, ComponentFuncs> m_component_factories;
        std::vector<SpawnEvent> m_spawn_events;

        struct EntityStackRecord
        {
            const char* debug_name;
            std::vector<uint32_t> allocated_entities;
        };
        std::vector<EntityStackRecord> m_entity_allocation_stack;
        bool m_full_release_on_next_sync;
    };
}
