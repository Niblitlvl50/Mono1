
#pragma once

#include "MonoFwd.h"
#include "Rendering/IDrawable.h"

namespace mono
{
    class PathBatchDrawer : public mono::IDrawable
    {
    public:
    
        PathBatchDrawer(const mono::PathSystem* path_system, const mono::TransformSystem* transform_system);
        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        const mono::PathSystem* m_path_system;
        const mono::TransformSystem* m_transform_system;
        bool m_enabled;
    };
}
