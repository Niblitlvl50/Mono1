

#include "EntitySystem.h"
#include "Util/Hash.h"
#include <numeric>
#include <algorithm>
#include <cassert>

using namespace mono;

EntitySystem::EntitySystem(size_t n_entities)
{
    m_entities.resize(n_entities);
    m_free_indices.resize(n_entities);
    m_debug_names.resize(n_entities);

    std::iota(m_free_indices.begin(), m_free_indices.end(), 0);
    std::reverse(m_free_indices.begin(), m_free_indices.end());
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

void EntitySystem::ReleaseEntity(uint32_t entity_id)
{
    m_entities[entity_id] = Entity();
    m_free_indices.push_back(entity_id);
}

Entity* EntitySystem::GetEntity(uint32_t entity_id)
{
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
