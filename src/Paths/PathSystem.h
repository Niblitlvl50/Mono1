
#pragma once

#include "MonoFwd.h"
#include "MonoPtrFwd.h"
#include "IGameSystem.h"

#include <vector>

namespace mono
{
    struct PathComponent
    {
        mono::IPathPtr path;
    };

    class PathSystem : public mono::IGameSystem
    {
    public:

        PathSystem(uint32_t n, mono::TransformSystem* transform_system);


        void AllocatePath(uint32_t entity_id);
        void ReleasePath(uint32_t entity_id);
        void SetPathData(uint32_t entity_id, const PathComponent& path_component);

        uint32_t Id() const override;
        const char* Name() const override;
        void Update(const mono::UpdateContext& update_context) override;

        mono::TransformSystem* m_transform_system;
        std::vector<PathComponent> m_path_components;
        std::vector<bool> m_active_paths;
    };
}
