
#include "EntitySystem.h"
#include "EntitySystem/Serialize.h"
#include "SystemContext.h"

#include "System/Hash.h"
#include "System/System.h"
#include "System/Uuid.h"
#include "System/Debug.h"
#include "System/File.h"
#include "Util/Algorithm.h"

#include <algorithm>
#include <vector>
#include <iterator>
#include <numeric>

#include "nlohmann/json.hpp"

namespace
{
    std::vector<mono::EntityData> LoadEntityFile(const char* entity_file)
    {
        System::Log("EntitySystem|Loading entity '%s'.", entity_file);

        std::vector<mono::EntityData> loaded_entities;

        file::FilePtr file = file::OpenAsciiFile(entity_file);
        if(!file)
        {
            System::Log("EntitySystem|Failed to load entity...");
            return loaded_entities;
        }

        std::vector<byte> file_data = file::FileRead(file);
        file_data.push_back('\0');

        const nlohmann::json& json = nlohmann::json::parse((const char*)file_data.data());
        const nlohmann::json& entities = json["entities"];

        for(const nlohmann::json& entity : entities)
        {
            const mono::EntityData& entity_data = entity;
            loaded_entities.push_back(entity_data);
        }

        return loaded_entities;
    }
}

using namespace mono;

AttributeNameLookupFunc EntitySystem::s_attribute_name_lookup = nullptr;

EntitySystem::EntitySystem(
    uint32_t n_entities,
    mono::SystemContext* system_context,
    ComponentNameLookupFunc component_lookup,
    AttributeNameLookupFunc attribute_lookup)
    : m_system_context(system_context)
    , m_component_name_lookup(component_lookup)
    , m_spawn_index(0)
    , m_full_release_on_next_sync(false)
{
    s_attribute_name_lookup = attribute_lookup;

    m_entities.resize(n_entities);
    m_entity_uuids.resize(n_entities, 0);
    m_entity_tags.resize(n_entities);
    m_free_indices.resize(n_entities);
    m_debug_names.resize(n_entities);
    m_release_callbacks.resize(n_entities);

    std::iota(m_free_indices.begin(), m_free_indices.end(), 0);
    std::reverse(m_free_indices.begin(), m_free_indices.end());
}

EntitySystem::~EntitySystem()
{
}

mono::Entity EntitySystem::CreateEntity(const char* name, const std::vector<uint32_t>& components)
{
    return CreateEntity(name, uuid::Uuid4Hashed(), components);
}

mono::Entity EntitySystem::CreateEntity(const char* name, uint32_t uuid_hash, const std::vector<uint32_t>& components)
{
    mono::Entity* new_entity = AllocateEntity(name);
    m_entity_uuids[new_entity->id] = uuid_hash;

    for(uint32_t component : components)
        AddComponent(new_entity->id, component);

    m_spawn_events.push_back({ true, new_entity->id });
    return *new_entity;
}

mono::Entity EntitySystem::SpawnEntity(const char* entity_file)
{
    const std::vector<mono::Entity>& loaded_entities = SpawnEntityCollection(entity_file);
    MONO_ASSERT(loaded_entities.size() == 1);
    return loaded_entities.front();
}

std::vector<mono::Entity> EntitySystem::SpawnEntityCollection(const char* entity_file)
{
    m_spawn_index++;

    const uint32_t entity_hash = hash::Hash(entity_file);
    const auto it = m_cached_entities.find(entity_hash);
    if(it == m_cached_entities.end())
        m_cached_entities[entity_hash] = LoadEntityFile(entity_file);

    std::vector<mono::Entity> loaded_entities;

    const std::vector<EntityData>& cached_entity_data = m_cached_entities[entity_hash];
    for(const EntityData& entity_data : cached_entity_data)
    {
        mono::Entity* new_entity = AllocateEntity(entity_data.entity_name.c_str());
        m_entity_uuids[new_entity->id] = entity_data.entity_uuid;
        new_entity->properties = entity_data.entity_properties;

        m_spawn_events.push_back({ true, new_entity->id });
        loaded_entities.push_back(*new_entity);
    }

    // Need to make a copy of component.properties somewhere so that i can patch
    // the event types with unique id, append an spawn index or something. At least 
    // for local events, and entity events.

    for(uint32_t index = 0; index < loaded_entities.size(); ++index)
    {
        const mono::Entity& entity = loaded_entities[index];
        const EntityData& entity_data = cached_entity_data[index];

        for(const ComponentData& component : entity_data.entity_components)
        {
            const uint32_t component_hash = hash::Hash(component.name.c_str());
            if(AddComponent(entity.id, component_hash))
            {
                std::vector<Attribute> local_attributes = component.properties;

                for(Attribute& attribute : local_attributes)
                {
                    mono::Event* event = std::get_if<mono::Event>(&attribute.value);
                    if(event)
                    {
                        if(event->type == mono::EventType::Global)
                        {
                            // Do nothing...
                        }
                        else if(event->type == mono::EventType::Local)
                        {
                            // Make event string unique to spawn
                            event->text += std::to_string(m_spawn_index);
                        }
                        else if(event->type == mono::EventType::Entity)
                        {
                            // Make event string unique to entity name
                            event->text += entity_file;
                        }
                    }
                }

                SetComponentData(entity.id, component_hash, local_attributes);
            }
        }
    }

    return loaded_entities;
}

bool EntitySystem::AddComponent(uint32_t entity_id, uint32_t component_hash)
{
    mono::Entity* entity = GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
    {
        const bool success = factory_it->second.create(entity, m_system_context);
        if(success)
        {
            /*
            const bool duplicated_component = mono::contains(entity->components, component_hash);
            if(duplicated_component)
            {
                const char* component_name = m_component_name_lookup(component_hash);
                System::Log("EntitySystem|Adding a duplicated component '%s' to entity '[%u] %s'", component_name, entity_id, entity->name);
            }
            */
            entity->components.push_back(component_hash);
        }
        return success;
    }

    //const char* component_name = m_component_name_lookup(component_hash);
    //System::Log("EntitySystem|There is no component registered with hash '%ul', %s", component_hash, component_name);
    return false;
}

bool EntitySystem::RemoveComponent(uint32_t entity_id, uint32_t component_hash)
{
    mono::Entity* entity = GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
    {
        const bool success = factory_it->second.release(entity, m_system_context);
        return success && mono::remove(entity->components, component_hash);
    }

    const char* component_name = m_component_name_lookup(component_hash);
    System::Log("EntitySystem|Unable to remove component with hash: %ul, %s", component_hash, component_name);
    return false;
}

bool EntitySystem::SetComponentData(uint32_t entity_id, uint32_t component_hash, const std::vector<Attribute>& properties)
{
    mono::Entity* entity = GetEntity(entity_id);

    const auto factory_it = m_component_factories.find(component_hash);
    if(factory_it != m_component_factories.end())
        return factory_it->second.update(entity, properties, m_system_context);

    //const char* component_name = m_component_name_lookup(component_hash);
    //System::Log("EntitySystem|Unable to update component with hash: %ul, %s", component_hash, component_name);
    return false;
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

void EntitySystem::SetEntityName(uint32_t entity_id, const char* name)
{
    SetName(entity_id, name);
}

const char* EntitySystem::GetEntityName(uint32_t entity_id) const
{
    if(entity_id == mono::INVALID_ID)
        return "";

    const std::string& name = GetName(entity_id);
    return name.c_str();
}

void EntitySystem::SetEntityTags(uint32_t entity_id, const std::vector<uint32_t>& tags)
{
    m_entity_tags[entity_id] = tags;
}

bool EntitySystem::HasEntityTag(uint32_t entity_id, uint32_t tag) const
{
    return mono::contains(m_entity_tags[entity_id], tag);
}

void EntitySystem::SetEntityEnabled(uint32_t entity_id, bool enable)
{
    mono::Entity* entity = GetEntity(entity_id);

    for(uint32_t component_hash : entity->components)
    {
        const auto factory_it = m_component_factories.find(component_hash);
        if(factory_it != m_component_factories.end() && factory_it->second.enable != nullptr)
            factory_it->second.enable(entity, enable, m_system_context);
    }
}

uint32_t EntitySystem::GetEntityUuid(uint32_t entity_id) const
{
    return m_entity_uuids[entity_id];
}

uint32_t EntitySystem::GetEntityIdFromUuid(uint32_t uuid) const
{
    const auto find_id = [uuid](uint32_t other_uuid) {
        return uuid == other_uuid;
    };

    const auto it = std::find_if(m_entity_uuids.begin(), m_entity_uuids.end(), find_id);
    if(it != m_entity_uuids.end())
        return std::distance(m_entity_uuids.begin(), it);

    return mono::INVALID_ID;
}

void EntitySystem::RegisterComponent(
    uint32_t component_hash,
    ComponentCreateFunc create_component,
    ComponentReleaseFunc release_component,
    ComponentUpdateFunc update_component,
    ComponentEnableFunc enable_component)
{
    m_component_factories[component_hash] = {
        create_component,
        release_component,
        update_component,
        enable_component,
    };
}

void EntitySystem::ReleaseEntity(uint32_t entity_id)
{
    m_entities_to_release.insert(entity_id);
    m_spawn_events.push_back({ false, entity_id });
}

void EntitySystem::ReleaseEntities(const std::vector<mono::Entity>& entities)
{
    for(const mono::Entity& entity : entities)
        ReleaseEntity(entity.id);
}

void EntitySystem::PushEntityStackRecord(const char* debug_name)
{
    EntityStackRecord record;
    record.debug_name = debug_name;

    const auto collect_active_entities = [&](Entity& entity) {
        record.allocated_entities.push_back(entity.id);
    };
    ForEachEntity(collect_active_entities);

    m_entity_allocation_stack.push_back(record);

//    System::Log("entitysystem|Found '%u' previously active entities when pushing stack record '%s'", record.allocated_entities.size(), debug_name);
//    for(uint32_t id : record.allocated_entities)
//        System::Log("\t[%u] %s", id, GetEntityName(id));
}

void EntitySystem::PopEntityStackRecord()
{
    std::vector<uint32_t> allocated_entities;

    const auto collect_active_entities = [&](Entity& entity) {
        allocated_entities.push_back(entity.id);
    };
    ForEachEntity(collect_active_entities);

    const EntityStackRecord record = m_entity_allocation_stack.back();

    std::vector<uint32_t> diff_result;
    std::set_difference(
        allocated_entities.begin(),
        allocated_entities.end(),
        record.allocated_entities.begin(),
        record.allocated_entities.end(),
        std::back_inserter(diff_result));

    //System::Log("entitysystem|Found '%u' that needs to be cleaned up, '%u' that stays. '%s'", diff_result.size(), record.allocated_entities.size(), record.debug_name);

    for(uint32_t id : diff_result)
    {
        //System::Log("\t[%u] %s", id, GetEntityName(id));
        ReleaseEntity(id);
    }

    m_entity_allocation_stack.pop_back();
    m_full_release_on_next_sync = true;
}

uint32_t EntitySystem::AddReleaseCallback(uint32_t entity_id, uint32_t callback_phases, const ReleaseCallback& callback)
{
    ReleaseCallbacks& callbacks = m_release_callbacks[entity_id];
    const uint32_t free_index = FindFreeCallbackIndex(callbacks);
    MONO_ASSERT(free_index != std::numeric_limits<uint32_t>::max());
    callbacks[free_index] = { callback_phases, callback };
    return free_index;
}

void EntitySystem::RemoveReleaseCallback(uint32_t entity_id, uint32_t callback_id)
{
    m_release_callbacks[entity_id][callback_id].callback = nullptr;
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

void EntitySystem::Reset()
{
    Sync();
}

void EntitySystem::Destroy()
{
    Sync();
}

void EntitySystem::DeferredRelease()
{
    const auto execute_callbacks = [](uint32_t entity_id, const ReleaseCallbacks& callbacks, mono::ReleasePhase phase) {

        for(const ReleaseCallbackData& callback_data : callbacks)
        {
            const bool valid_callback_type = (callback_data.callback_types & phase);
            if(!valid_callback_type || callback_data.callback == nullptr)
                continue;

            callback_data.callback(entity_id, phase);
        }
    };

    std::unordered_set<uint32_t> local_entities_to_release = m_entities_to_release;
    m_entities_to_release.clear();

    for(uint32_t entity_id : local_entities_to_release)
    {
        ReleaseCallbacks& callbacks = m_release_callbacks[entity_id];
        execute_callbacks(entity_id, callbacks, mono::ReleasePhase::PRE_RELEASE);

        mono::Entity* entity = GetEntity(entity_id);
        std::reverse(entity->components.begin(), entity->components.end()); // We should release the components in reverse.

        for(uint32_t component_hash : entity->components)
        {
            const auto factory_it = m_component_factories.find(component_hash);
            if(factory_it != m_component_factories.end())
                factory_it->second.release(entity, m_system_context);
            else
                System::Log("entitysystem|Found component hash but no release function. Hash: %u", component_hash);
        }

        m_entities[entity_id] = Entity();
        m_debug_names[entity_id].clear();

        execute_callbacks(entity_id, callbacks, mono::ReleasePhase::POST_RELEASE);
        for(ReleaseCallbackData& callback_data : callbacks)
            callback_data.callback = nullptr;

        m_free_indices.push_back(entity_id);
    }

    if(!m_entities_to_release.empty())
    {
        //System::Log("entitysystem|Additional entities to release after a deferred release. %u", m_entities_to_release.size());
        //for(uint32_t id : m_entities_to_release)
        //    System::Log("\t[%u] %s", id, GetEntityName(id));
    
        if(m_full_release_on_next_sync)
        {
            m_entities_to_release.clear();
            m_full_release_on_next_sync = false;
        }
    }
}

uint32_t EntitySystem::FindFreeCallbackIndex(const ReleaseCallbacks& callbacks) const
{
    for(uint32_t index = 0; index < std::size(callbacks); ++index)
    {
        if(callbacks[index].callback == nullptr)
            return index;
    }

    return std::numeric_limits<uint32_t>::max();
}

Entity* EntitySystem::AllocateEntity(const char* name)
{
    MONO_ASSERT(!m_free_indices.empty());

    const uint32_t entity_id = m_free_indices.back();
    m_free_indices.pop_back();

    Entity& entity = m_entities[entity_id];
    MONO_ASSERT(entity.id == INVALID_ID);
    MONO_ASSERT(entity.components.empty());
    MONO_ASSERT(m_debug_names[entity_id].empty());

    m_debug_names[entity_id] = name;

    entity.id = entity_id;
    entity.name = m_debug_names[entity_id].c_str();

    return &entity;
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

std::vector<uint32_t> EntitySystem::CollectEntitiesWithTag(uint32_t tag) const
{
    std::vector<uint32_t> found_entities;

    for(uint32_t index = 0; index < m_entity_tags.size(); ++index)
    {
        const std::vector<uint32_t>& tags = m_entity_tags[index];
        if(mono::contains(tags, tag))
            found_entities.push_back(index);
    }

    return found_entities;
}

const char* EntitySystem::Name() const
{
    return "entitysystem";
}

void EntitySystem::Update(const UpdateContext& update_context)
{ }

const char* EntitySystem::AttributeNameLookup(uint32_t attribute_type_hash)
{
    if(s_attribute_name_lookup)
        return s_attribute_name_lookup(attribute_type_hash);
    return "Unknown";
}
