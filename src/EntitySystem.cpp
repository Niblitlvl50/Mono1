

#include "EntitySystem.h"
#include "Hash.h"
#include <numeric>
#include <algorithm>
#include <cassert>

using namespace mono;

EntitySystem::EntitySystem(size_t n_entities)
{
    m_entities.resize(n_entities);
    m_alive.resize(n_entities, false);
    m_free_indices.resize(n_entities);
    m_debug_names.resize(n_entities);

    std::iota(m_free_indices.begin(), m_free_indices.end(), 0);
    std::reverse(m_free_indices.begin(), m_free_indices.end());
}

Entity& EntitySystem::AllocateEntity()
{
    const uint32_t entity_id = m_free_indices.back();
    m_free_indices.pop_back();

    assert(!m_alive[entity_id]);
    m_alive[entity_id] = true;

    Entity& entity = m_entities[entity_id];
    entity.id = entity_id;

    return entity;
}

void EntitySystem::ReleaseEntity(uint32_t entity_id)
{
    assert(m_alive[entity_id]);
    m_alive[entity_id] = false;

    m_entities[entity_id] = Entity();
    m_free_indices.push_back(entity_id);
}

Entity& EntitySystem::GetEntity(uint32_t entity_id)
{
    return m_entities[entity_id];
}

void EntitySystem::SetProperty(Entity entity, uint32_t property)
{
    m_entities[entity.id].properties |= property;
}

bool EntitySystem::HasProperty(Entity entity, uint32_t property) const
{
    return m_entities[entity.id].properties & property;
}

void EntitySystem::SetName(uint32_t entity_id, const std::string& name)
{
    m_debug_names[entity_id] = name;
}

const std::string& EntitySystem::GetName(uint32_t entity_id) const
{
    return m_debug_names[entity_id];
}

uint32_t EntitySystem::Id() const
{
    return mono::Hash(Name());
}

const char* EntitySystem::Name() const
{
    return "entitysystem";
}

uint32_t EntitySystem::Capacity() const
{
    return m_entities.size();
}

void EntitySystem::Update(uint32_t delta_ms)
{ }
