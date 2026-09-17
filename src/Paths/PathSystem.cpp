
#include "PathSystem.h"
#include "PathFactory.h"

#include "TransformSystem/TransformSystem.h"
#include "System/Hash.h"

#include <algorithm>

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
    m_dirty_components.push_back(entity_id);
}

const PathComponent* PathSystem::GetPath(uint32_t entity_id) const
{
    return &m_path_components[entity_id];
}

PathNotifierComponent* PathSystem::AllocateNotifier(uint32_t entity_id)
{
    std::vector<PathNotifierComponent>& notifiers = m_notifiers[entity_id];
    notifiers.emplace_back();
    return &notifiers.back();
}

void PathSystem::ReleaseNotifier(uint32_t entity_id)
{
    m_notifiers.erase(entity_id);
}

void PathSystem::SetNotifierData(uint32_t entity_id, float distance, const std::string& tag)
{
    const auto it = m_notifiers.find(entity_id);
    if(it == m_notifiers.end() || it->second.empty())
        return;

    // SetNotifierData always follows the AllocateNotifier call for the same instance.
    PathNotifierComponent& notifier = it->second.back();
    notifier.distance = distance;
    notifier.tag = tag;

    // Keep notifiers ordered by distance so callers can walk "what's next" in sequence.
    std::sort(it->second.begin(), it->second.end(), [](const PathNotifierComponent& a, const PathNotifierComponent& b) {
        return a.distance < b.distance;
    });
}

const std::vector<PathNotifierComponent>* PathSystem::GetNotifiers(uint32_t entity_id) const
{
    const auto it = m_notifiers.find(entity_id);
    return (it != m_notifiers.end()) ? &it->second : nullptr;
}

std::vector<PathNotifierComponent> PathSystem::CollectNotifiersInRange(
    uint32_t path_entity_id, float min_distance, float max_distance) const
{
    std::vector<PathNotifierComponent> result;

    const auto it = m_notifiers.find(path_entity_id);
    if(it == m_notifiers.end())
        return result;

    for(const PathNotifierComponent& notifier : it->second)
    {
        if(notifier.distance >= min_distance && notifier.distance <= max_distance)
            result.push_back(notifier);
    }

    return result;
}

const char* PathSystem::Name() const
{
    return "pathsystem";
}

void PathSystem::Update(const mono::UpdateContext& update_context)
{
    for(uint32_t index = 0; index < m_active_paths.size(); ++index)
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

void PathSystem::Sync()
{
    for(uint32_t entity_id : m_dirty_components)
    {
        for(size_t index = 0; index < std::size(m_callbacks); ++index)
        {
            PathUpdatedCallback& callback = m_callbacks[index];
            if(callback)
                callback(entity_id);
        }
    }

    m_dirty_components.clear();
}

uint32_t PathSystem::RegisterDirtyCallback(const PathUpdatedCallback& callback)
{
    const auto is_free = [](const PathUpdatedCallback& callback) {
        return callback == nullptr;
    };

    auto it = std::find_if(std::begin(m_callbacks), std::end(m_callbacks), is_free);
    if(it != std::end(m_callbacks))
    {
        (*it) = callback;
        return std::distance(std::begin(m_callbacks), it);
    }

    return -1;
}

void PathSystem::RemoveDirtyCallback(uint32_t id)
{
    m_callbacks[id] = nullptr;
}
