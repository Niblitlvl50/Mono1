
#include "RiverSystem.h"

using namespace mono;

RiverSystem::RiverSystem(uint32_t n)
{
    m_active.resize(n, false);
    m_rivers.resize(n);
}

RiverComponent* RiverSystem::Allocate(uint32_t entity_id)
{
    m_active[entity_id] = true;
    return &m_rivers[entity_id];
}

void RiverSystem::Release(uint32_t entity_id)
{
    m_active[entity_id] = false;
}

void RiverSystem::SetData(uint32_t entity_id, const RiverComponent& component)
{
    m_rivers[entity_id] = component;
}

const char* RiverSystem::Name() const
{
    return "riversystem";
}

void RiverSystem::Update(const mono::UpdateContext& update_context)
{
}
