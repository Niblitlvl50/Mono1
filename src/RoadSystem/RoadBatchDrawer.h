
#pragma once

#include "MonoFwd.h"
#include "Rendering/IDrawable.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Paths/PathDrawBuffer.h"

#include <unordered_map>
#include <string>

namespace mono
{
    class RoadSystem;
    struct RoadComponent;

    class RoadBatchDrawer : public mono::IDrawable
    {
    public:

        RoadBatchDrawer(const RoadSystem* road_system, mono::PathSystem* path_system, const mono::TransformSystem* transform_system);
        ~RoadBatchDrawer();

    private:

        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        struct CachedRoad
        {
            bool dirty;
            float width;
            std::string texture_name;
            mono::ITexturePtr texture;
            mono::PathDrawBuffer buffers;
        };

        CachedRoad CacheRoadData(uint32_t entity_id, const RoadComponent& component) const;
        bool NeedsUpdate(const CachedRoad& road, const RoadComponent& component) const;

        const RoadSystem* m_road_system;
        mono::PathSystem* m_path_system;
        const mono::TransformSystem* m_transform_system;
        uint32_t m_callback_id;

        mutable std::unordered_map<uint32_t, CachedRoad> m_cached_roads;
    };
}
