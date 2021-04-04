
#include "PathSystem.h"
#include "PathFactory.h"

#include "TransformSystem/TransformSystem.h"
#include "Util/Hash.h"

using namespace mono;

PathSystem::PathSystem(uint32_t n, mono::TransformSystem* transform_system)
    : m_transform_system(transform_system)
{
    m_path_components.resize(n);
    m_active_paths.resize(n, false);
}

PathComponent* PathSystem::AllocatePath(uint32_t entity_id)
{
    m_active_paths[entity_id] = true;
    return &m_path_components[entity_id];
}

void PathSystem::ReleasePath(uint32_t entity_id)
{
    m_active_paths[entity_id] = false;
}

void PathSystem::SetPathData(uint32_t entity_id, const PathComponent& path_component)
{
    m_path_components[entity_id] = path_component;
}

const PathComponent* PathSystem::GetPath(uint32_t entity_id)
{
    return &m_path_components[entity_id];
}

uint32_t PathSystem::Id() const
{
    return mono::Hash(Name());
}

const char* PathSystem::Name() const
{
    return "pathsystem";
}

void PathSystem::Update(const mono::UpdateContext& update_context)
{
    for(size_t index = 0; index < m_active_paths.size(); ++index)
    {
        const bool is_active = m_active_paths[index];
        if(!is_active)
            continue;

        math::Vector min = { math::INF, math::INF };
        math::Vector max = { -math::INF, -math::INF };

        const PathComponent& component = m_path_components[index];
        for(const math::Vector& point : component.points)
        {
            min.x = std::min(min.x, point.x);
            min.y = std::min(min.y, point.y);

            max.x = std::max(max.x, point.x);
            max.y = std::max(max.y, point.y);
        }

        math::Quad& local_bb = m_transform_system->GetBoundingBox(index);
        local_bb = math::Quad(min, max);
    }
}
