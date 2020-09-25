
#include "EntityManager.h"

#include "Util/Hash.h"
#include "SystemContext.h"
#include "System/System.h"
#include "Util/Algorithm.h"

#include <algorithm>
#include <vector>

using namespace mono;

EntityManager::EntityManager(mono::SystemContext* system_context)
    : m_system_context(system_context)
{ }

EntityManager::~EntityManager()
{
    Sync();
}

mono::Entity EntityManager::CreateEntity(const char* name, const std::vector<uint32_t>& components)
{
    mono::EntitySystem* entity_system = m_system_context->GetSystem<mono::EntitySystem>();
    mono::Entity* new_entity = entity_system->AllocateEntity();
    entity_system->SetName(new_entity->id, name);
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
        m_cached_entities[entity_hash] = LoadEntityFile(entity_file);

    mono::EntitySystem* entity_system = m_system_context->GetSystem<mono::EntitySystem>();
    mono::Entity* new_entity = entity_system->AllocateEntity();

    const EntityData& entity_data = m_cached_entities[entity_hash];
    entity_system->SetName(new_entity->id, entity_data.entity_name);
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
    mono::EntitySystem* entity_system = m_system_context->GetSystem<mono::EntitySystem>();
    mono::Entity* entity = entity_system->GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
    {
        const bool success = factory_it->second.create(entity, m_system_context);
        if(success)
            entity->components.push_back(component_hash);
        return success;
    }

    System::Log("EntityManager|There is no component registered with hash '%ul', %s\n", component_hash, ComponentNameFromHash(component_hash));
    return false;
}

bool EntityManager::RemoveComponent(uint32_t entity_id, uint32_t component_hash)
{
    mono::EntitySystem* entity_system = m_system_context->GetSystem<mono::EntitySystem>();
    mono::Entity* entity = entity_system->GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
    {
        const bool success = factory_it->second.release(entity, m_system_context);
        if(success)
            mono::remove(entity->components, component_hash);
        return success;
    }

    System::Log("EntityManager|Unable to remove component with hash: %ul, %s\n", component_hash, ComponentNameFromHash(component_hash));
    return false;
}

bool EntityManager::SetComponentData(uint32_t entity_id, uint32_t component_hash, const std::vector<Attribute>& properties)
{
    mono::EntitySystem* entity_system = m_system_context->GetSystem<mono::EntitySystem>();
    mono::Entity* entity = entity_system->GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
        return factory_it->second.update(entity, properties, m_system_context);

    System::Log("EntityManager|Unable to update component with hash: %ul, %s\n", component_hash, ComponentNameFromHash(component_hash));
    return false;
}

std::vector<Attribute> EntityManager::GetComponentData(uint32_t entity_id, uint32_t component_hash) const
{
    mono::EntitySystem* entity_system = m_system_context->GetSystem<mono::EntitySystem>();
    mono::Entity* entity = entity_system->GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
    {
        ComponentGetFunc get_func = factory_it->second.get;
        if(get_func)
            return factory_it->second.get(entity, m_system_context);
    }

    System::Log("EntityManager|Unable to get component with hash: %ul, %s\n", component_hash, ComponentNameFromHash(component_hash));
    return { };
}

void EntityManager::SetEntityProperties(uint32_t entity_id, uint32_t properties)
{
    mono::EntitySystem* entity_system = m_system_context->GetSystem<mono::EntitySystem>();
    mono::Entity* entity = entity_system->GetEntity(entity_id);
    entity->properties = properties;
}

uint32_t EntityManager::GetEntityProperties(uint32_t entity_id) const
{
    mono::EntitySystem* entity_system = m_system_context->GetSystem<mono::EntitySystem>();
    mono::Entity* entity = entity_system->GetEntity(entity_id);

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

    mono::EntitySystem* entity_system = m_system_context->GetSystem<mono::EntitySystem>();

    for(uint32_t entity_id : local_entities_to_release)
    {
        mono::Entity* entity = entity_system->GetEntity(entity_id);
        for(uint32_t component_hash : entity->components)
        {
            const auto factory_it = m_component_factories.find(component_hash);
            if(factory_it != m_component_factories.end())
                factory_it->second.release(entity, m_system_context);
        }

        entity_system->ReleaseEntity(entity_id);
    }
}
