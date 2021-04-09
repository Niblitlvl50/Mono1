
#pragma once

#include "MonoFwd.h"
#include "Math/MathFwd.h"
#include "PathTypes.h"
#include "IGameSystem.h"

#include <vector>
#include <functional>

namespace mono
{
    struct PathComponent
    {
        PathType type;
        bool closed;
        std::vector<math::Vector> points;
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

        uint32_t Id() const override;
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
    };
}
