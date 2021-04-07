
#include "RoadSystem.h"
#include "Util/Hash.h"

using namespace mono;

RoadSystem::RoadSystem(uint32_t n)
{
    m_active.resize(n, false);
    m_roads.resize(n);
}

RoadComponent* RoadSystem::Allocate(uint32_t entity_id)
{
    m_active[entity_id] = true;
    return &m_roads[entity_id];
}

void RoadSystem::Release(uint32_t entity_id)
{
    m_active[entity_id] = false;
}

void RoadSystem::SetData(uint32_t entity_id, const RoadComponent& component)
{
    m_roads[entity_id] = component;
}

uint32_t RoadSystem::Id() const
{
    return mono::Hash(Name());
}

const char* RoadSystem::Name() const
{
    return "roadsystem";
}

void RoadSystem::Update(const mono::UpdateContext& update_context)
{

}
