
#pragma once

#include "MonoFwd.h"
#include "Rendering/IDrawable.h"

namespace mono
{
    class RoadSystem;

    class RoadBatchDrawer : public mono::IDrawable
    {
    public:

        RoadBatchDrawer(const RoadSystem* road_system, const mono::PathSystem* path_system, const mono::TransformSystem* transform_system);
        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        const RoadSystem* m_road_system;
        const mono::PathSystem* m_path_system;
        const mono::TransformSystem* m_transform_system;
    };
}
