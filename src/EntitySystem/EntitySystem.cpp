
#include "EntitySystem.h"

#include "Util/Hash.h"
#include "SystemContext.h"
#include "System/System.h"
#include "Util/Algorithm.h"

#include <algorithm>
#include <vector>

#include <numeric>
#include <cassert>

using namespace mono;

EntitySystem::EntitySystem(
    uint32_t n_entities,
    mono::SystemContext* system_context,
    EntityLoadFunc load_func,
    ComponentNameLookupFunc component_lookup)
    : m_system_context(system_context)
    , m_load_func(load_func)
    , m_component_name_lookup(component_lookup)
    , m_ignore_releases(false)
{
    m_entities.resize(n_entities);
    m_free_indices.resize(n_entities);
    m_debug_names.resize(n_entities);

    std::iota(m_free_indices.begin(), m_free_indices.end(), 0);
    std::reverse(m_free_indices.begin(), m_free_indices.end());
}

EntitySystem::~EntitySystem()
{
    Sync();
}

mono::Entity EntitySystem::CreateEntity(const char* name, const std::vector<uint32_t>& components)
{
    mono::Entity* new_entity = AllocateEntity();
    SetName(new_entity->id, name);
    for(uint32_t component : components)
        AddComponent(new_entity->id, component);

    m_spawn_events.push_back({ true, new_entity->id });
    return *new_entity;
}

mono::Entity EntitySystem::CreateEntity(const char* entity_file)
{
    const uint32_t entity_hash = mono::Hash(entity_file);
    const auto it = m_cached_entities.find(entity_hash);
    if(it == m_cached_entities.end())
        m_cached_entities[entity_hash] = m_load_func(entity_file);

    const EntityData& entity_data = m_cached_entities[entity_hash];

    mono::Entity* new_entity = AllocateEntity();
    SetName(new_entity->id, entity_data.entity_name);
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

bool EntitySystem::AddComponent(uint32_t entity_id, uint32_t component_hash)
{
    mono::Entity* entity = GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
    {
        const bool success = factory_it->second.create(entity, m_system_context);
        if(success)
            entity->components.push_back(component_hash);
        return success;
    }

    const char* component_name = m_component_name_lookup(component_hash);
    System::Log("EntitySystem|There is no component registered with hash '%ul', %s\n", component_hash, component_name);
    return false;
}

bool EntitySystem::RemoveComponent(uint32_t entity_id, uint32_t component_hash)
{
    mono::Entity* entity = GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
    {
        const bool success = factory_it->second.release(entity, m_system_context);
        if(success)
            mono::remove(entity->components, component_hash);
        return success;
    }

    const char* component_name = m_component_name_lookup(component_hash);
    System::Log("EntitySystem|Unable to remove component with hash: %ul, %s\n", component_hash, component_name);
    return false;
}

bool EntitySystem::SetComponentData(uint32_t entity_id, uint32_t component_hash, const std::vector<Attribute>& properties)
{
    mono::Entity* entity = GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
        return factory_it->second.update(entity, properties, m_system_context);

    const char* component_name = m_component_name_lookup(component_hash);
    System::Log("EntitySystem|Unable to update component with hash: %ul, %s\n", component_hash, component_name);
    return false;
}

std::vector<Attribute> EntitySystem::GetComponentData(uint32_t entity_id, uint32_t component_hash) const
{
    const mono::Entity* entity = GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
    {
        ComponentGetFunc get_func = factory_it->second.get;
        if(get_func)
            return factory_it->second.get(entity, m_system_context);
    }

    const char* component_name = m_component_name_lookup(component_hash);
    System::Log("EntitySystem|Unable to get component with hash: %ul, %s\n", component_hash, component_name);
    return { };
}

void EntitySystem::SetEntityProperties(uint32_t entity_id, uint32_t properties)
{
    mono::Entity* entity = GetEntity(entity_id);
    entity->properties = properties;
}

uint32_t EntitySystem::GetEntityProperties(uint32_t entity_id) const
{
    const mono::Entity* entity = GetEntity(entity_id);
    return entity->properties;
}

const char* EntitySystem::GetEntityName(uint32_t entity_id) const
{
    const std::string& name = GetName(entity_id);
    return name.c_str();
}

void EntitySystem::RegisterComponent(
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

void EntitySystem::ReleaseEntity(uint32_t entity_id)
{
    if(m_ignore_releases)
        return;

    m_entities_to_release.insert(entity_id);
    m_spawn_events.push_back({ false, entity_id });
}

void EntitySystem::ReleaseAllEntities()
{
    const auto collect_active_entities = [this](Entity& entity) {
        ReleaseEntity(entity.id);
    };

    ForEachEntity(collect_active_entities);

    m_ignore_releases = true;
    Sync();
    m_ignore_releases = false;
}

const std::vector<EntitySystem::SpawnEvent>& EntitySystem::GetSpawnEvents() const
{
    return m_spawn_events;
}

void EntitySystem::Sync()
{
    DeferredRelease();
    m_spawn_events.clear();
}

void EntitySystem::DeferredRelease()
{
    std::unordered_set<uint32_t> local_entities_to_release = m_entities_to_release;
    m_entities_to_release.clear();

    for(uint32_t entity_id : local_entities_to_release)
    {
        mono::Entity* entity = GetEntity(entity_id);
        for(uint32_t component_hash : entity->components)
        {
            const auto factory_it = m_component_factories.find(component_hash);
            if(factory_it != m_component_factories.end())
                factory_it->second.release(entity, m_system_context);
        }

        ReleaseEntity2(entity_id);
    }
}



Entity* EntitySystem::AllocateEntity()
{
    const uint32_t entity_id = m_free_indices.back();
    m_free_indices.pop_back();

    Entity& entity = m_entities[entity_id];
    assert(entity.id == INVALID_ID);

    entity.id = entity_id;
    entity.name = m_debug_names[entity_id].c_str();

    return &entity;
}

void EntitySystem::ReleaseEntity2(uint32_t entity_id)
{
    m_entities[entity_id] = Entity();
    m_debug_names[entity_id].clear();
    m_free_indices.push_back(entity_id);
}

Entity* EntitySystem::GetEntity(uint32_t entity_id)
{
    if(entity_id >= m_entities.size())
        return nullptr;
    return &m_entities[entity_id];
}

const Entity* EntitySystem::GetEntity(uint32_t entity_id) const
{
    if(entity_id >= m_entities.size())
        return nullptr;
    return &m_entities[entity_id];
}

void EntitySystem::SetProperty(Entity entity, uint32_t property)
{
    m_entities[entity.id].properties |= property;
}

bool EntitySystem::HasProperty(Entity entity, uint32_t property) const
{
    return m_entities[entity.id].properties & property;
}

bool EntitySystem::HasComponent(const mono::Entity* entity, uint32_t component_hash) const
{
    const std::vector<uint32_t>& entity_components = entity->components;
    return (std::find(entity_components.begin(), entity_components.end(), component_hash) != entity_components.end());
}

void EntitySystem::SetName(uint32_t entity_id, const std::string& name)
{
    m_debug_names[entity_id] = name;
}

const std::string& EntitySystem::GetName(uint32_t entity_id) const
{
    return m_debug_names[entity_id];
}

uint32_t EntitySystem::FindEntityByName(const char* name) const
{
    const auto it = std::find(m_debug_names.begin(), m_debug_names.end(), name);
    if(it != m_debug_names.end())
        return std::distance(m_debug_names.begin(), it);

    return INVALID_ID;
}

uint32_t EntitySystem::Id() const
{
    return mono::Hash(Name());
}

const char* EntitySystem::Name() const
{
    return "entitysystem";
}

void EntitySystem::Update(const UpdateContext& update_context)
{ }
