
#pragma once

#include "MonoFwd.h"
#include "Math/MathFwd.h"
#include "PathTypes.h"
#include "IGameSystem.h"

#include <vector>
#include <functional>
#include <string>
#include <unordered_map>

namespace mono
{
    struct PathComponent
    {
        PathType type;
        bool closed;
        std::vector<math::Vector> points;
    };

    // A named tag at a distance along the path on the same entity - purely data, for
    // other code to query later (e.g. "what's coming up next on this track").
    struct PathNotifierComponent
    {
        float distance = 0.0f;
        std::string tag;
    };

    using PathUpdatedCallback = std::function<void (uint32_t)>;

    class PathSystem : public mono::IGameSystem
    {
    public:

        PathSystem(uint32_t n, mono::TransformSystem* transform_system);

        PathComponent* AllocatePath(uint32_t entity_id);
        void ReleasePath(uint32_t entity_id);
        void SetPathData(uint32_t entity_id, const PathComponent& path_component);

        const PathComponent* GetPath(uint32_t entity_id) const;

        // A path entity can carry several notifiers (allow_multiple); each Allocate call
        // appends one, and the following SetNotifierData call fills in the one just added.
        PathNotifierComponent* AllocateNotifier(uint32_t entity_id);
        void ReleaseNotifier(uint32_t entity_id);
        void SetNotifierData(uint32_t entity_id, float distance, const std::string& tag);
        const std::vector<PathNotifierComponent>* GetNotifiers(uint32_t entity_id) const;

        // Notifiers on `path_entity_id` whose distance falls within [min_distance, max_distance].
        std::vector<PathNotifierComponent> CollectNotifiersInRange(uint32_t path_entity_id, float min_distance, float max_distance) const;

        uint32_t FindPathFromNotifierTag(const std::string& tag, float& out_distance) const;

        template <typename T>
        inline void ForEachNotifier(T&& callback) const
        {
            for(const auto& entity_notifiers_pair : m_notifiers)
            {
                for(const PathNotifierComponent& notifier : entity_notifiers_pair.second)
                    callback(entity_notifiers_pair.first, notifier);
            }
        }

        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;
        void Sync() override;

        uint32_t RegisterDirtyCallback(const PathUpdatedCallback& callback);
        void RemoveDirtyCallback(uint32_t id);

        template <typename T>
        inline void ForEach(T&& callback) const
        {
            for(uint32_t index = 0; index < m_active_paths.size(); ++index)
            {
                if(m_active_paths[index])
                    callback(m_path_components[index], index);
            }
        }

    private:

        mono::TransformSystem* m_transform_system;
        std::vector<PathComponent> m_path_components;
        std::vector<bool> m_active_paths;

        std::vector<uint32_t> m_dirty_components;
        PathUpdatedCallback m_callbacks[8];

        std::unordered_map<uint32_t, std::vector<PathNotifierComponent>> m_notifiers;
    };
}
