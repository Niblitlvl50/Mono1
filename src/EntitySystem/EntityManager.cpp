
#include "EntityManager.h"

#include "Util/Hash.h"
#include "SystemContext.h"
#include "System/System.h"
#include "Util/Algorithm.h"

#include <algorithm>
#include <vector>

using namespace mono;

EntityManager::EntityManager(
    mono::EntitySystem* entity_system,
    mono::SystemContext* system_context,
    EntityLoadFunc load_func,
    ComponentNameLookupFunc component_lookup)
    : m_entity_system(entity_system)
    , m_system_context(system_context)
    , m_load_func(load_func)
    , m_component_name_lookup(component_lookup)
{ }

EntityManager::~EntityManager()
{
    Sync();
}

mono::Entity EntityManager::CreateEntity(const char* name, const std::vector<uint32_t>& components)
{
    mono::Entity* new_entity = m_entity_system->AllocateEntity();
    m_entity_system->SetName(new_entity->id, name);
    for(uint32_t component : components)
        AddComponent(new_entity->id, component);

    m_spawn_events.push_back({ true, new_entity->id });
    return *new_entity;
}

mono::Entity EntityManager::CreateEntity(const char* entity_file)
{
    const uint32_t entity_hash = mono::Hash(entity_file);
    const auto it = m_cached_entities.find(entity_hash);
    if(it == m_cached_entities.end())
        m_cached_entities[entity_hash] = m_load_func(entity_file);

    const EntityData& entity_data = m_cached_entities[entity_hash];

    mono::Entity* new_entity = m_entity_system->AllocateEntity();
    m_entity_system->SetName(new_entity->id, entity_data.entity_name);
    new_entity->properties = entity_data.entity_properties;

    for(const ComponentData& component : entity_data.entity_components)
    {
        const uint32_t component_hash = mono::Hash(component.name.c_str());
        if(AddComponent(new_entity->id, component_hash))
            SetComponentData(new_entity->id, component_hash, component.properties);
    }

    m_spawn_events.push_back({ true, new_entity->id });
    return *new_entity;
}

bool EntityManager::AddComponent(uint32_t entity_id, uint32_t component_hash)
{
    mono::Entity* entity = m_entity_system->GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
    {
        const bool success = factory_it->second.create(entity, m_system_context);
        if(success)
            entity->components.push_back(component_hash);
        return success;
    }

    const char* component_name = m_component_name_lookup(component_hash);
    System::Log("EntityManager|There is no component registered with hash '%ul', %s\n", component_hash, component_name);
    return false;
}

bool EntityManager::RemoveComponent(uint32_t entity_id, uint32_t component_hash)
{
    mono::Entity* entity = m_entity_system->GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
    {
        const bool success = factory_it->second.release(entity, m_system_context);
        if(success)
            mono::remove(entity->components, component_hash);
        return success;
    }

    const char* component_name = m_component_name_lookup(component_hash);
    System::Log("EntityManager|Unable to remove component with hash: %ul, %s\n", component_hash, component_name);
    return false;
}

bool EntityManager::SetComponentData(uint32_t entity_id, uint32_t component_hash, const std::vector<Attribute>& properties)
{
    mono::Entity* entity = m_entity_system->GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
        return factory_it->second.update(entity, properties, m_system_context);

    const char* component_name = m_component_name_lookup(component_hash);
    System::Log("EntityManager|Unable to update component with hash: %ul, %s\n", component_hash, component_name);
    return false;
}

std::vector<Attribute> EntityManager::GetComponentData(uint32_t entity_id, uint32_t component_hash) const
{
    mono::Entity* entity = m_entity_system->GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
    {
        ComponentGetFunc get_func = factory_it->second.get;
        if(get_func)
            return factory_it->second.get(entity, m_system_context);
    }

    const char* component_name = m_component_name_lookup(component_hash);
    System::Log("EntityManager|Unable to get component with hash: %ul, %s\n", component_hash, component_name);
    return { };
}

void EntityManager::SetEntityProperties(uint32_t entity_id, uint32_t properties)
{
    mono::Entity* entity = m_entity_system->GetEntity(entity_id);
    entity->properties = properties;
}

uint32_t EntityManager::GetEntityProperties(uint32_t entity_id) const
{
    mono::Entity* entity = m_entity_system->GetEntity(entity_id);
    return entity->properties;
}

void EntityManager::RegisterComponent(
    uint32_t component_hash,
    ComponentCreateFunc create_component,
    ComponentReleaseFunc release_component,
    ComponentUpdateFunc update_component,
    ComponentGetFunc get_component)
{
    m_component_factories[component_hash] = {
        create_component,
        release_component,
        update_component,
        get_component
    };
}

void EntityManager::ReleaseEntity(uint32_t entity_id)
{
    m_entities_to_release.insert(entity_id);
    m_spawn_events.push_back({ false, entity_id });
}

void EntityManager::ReleaseAllEntities()
{
    const auto collect_active_entities = [this](Entity& entity) {
        ReleaseEntity(entity.id);
    };

    m_entity_system->ForEachEntity(collect_active_entities);
}

const std::vector<EntityManager::SpawnEvent>& EntityManager::GetSpawnEvents() const
{
    return m_spawn_events;
}

void EntityManager::Sync()
{
    DeferredRelease();
    m_spawn_events.clear();
}

void EntityManager::DeferredRelease()
{
    std::unordered_set<uint32_t> local_entities_to_release = m_entities_to_release;
    m_entities_to_release.clear();

    for(uint32_t entity_id : local_entities_to_release)
    {
        mono::Entity* entity = m_entity_system->GetEntity(entity_id);
        for(uint32_t component_hash : entity->components)
        {
            const auto factory_it = m_component_factories.find(component_hash);
            if(factory_it != m_component_factories.end())
                factory_it->second.release(entity, m_system_context);
        }

        m_entity_system->ReleaseEntity(entity_id);
    }
}
