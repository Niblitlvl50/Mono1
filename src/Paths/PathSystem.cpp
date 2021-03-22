
#include "PathSystem.h"

#include "Paths/IPath.h"
#include "TransformSystem/TransformSystem.h"
#include "Util/Hash.h"

using namespace mono;

PathSystem::PathSystem(uint32_t n, mono::TransformSystem* transform_system)
    : m_transform_system(transform_system)
{
    m_path_components.resize(n);
    m_active_paths.resize(n, false);
}

void PathSystem::AllocatePath(uint32_t entity_id)
{
    
}

void PathSystem::ReleasePath(uint32_t entity_id)
{

}

void PathSystem::SetPathData(uint32_t entity_id, const PathComponent& path_component)
{

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

}
