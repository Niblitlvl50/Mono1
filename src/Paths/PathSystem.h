
#pragma once

#include "MonoFwd.h"
#include "Math/MathFwd.h"
#include "PathTypes.h"
#include "IGameSystem.h"

#include <vector>

namespace mono
{
    struct PathComponent
    {
        PathType type;
        float width;
        bool closed;
        std::vector<math::Vector> points;
    };

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
    };
}
