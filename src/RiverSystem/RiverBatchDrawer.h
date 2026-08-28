
#pragma once

#include "MonoFwd.h"
#include "Rendering/IDrawable.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Paths/PathDrawBuffer.h"

#include <unordered_map>
#include <string>

namespace mono
{
    class RiverSystem;
    struct RiverComponent;

    class RiverBatchDrawer : public mono::IDrawable
    {
    public:

        RiverBatchDrawer(const RiverSystem* river_system, mono::PathSystem* path_system, const mono::TransformSystem* transform_system);
        ~RiverBatchDrawer();

    private:

        void Draw(mono::IRenderer& renderer) const override;
        math::Quad BoundingBox() const override;

        struct CachedRiver
        {
            bool dirty;
            float width;
            std::string texture_name;
            mono::ITexturePtr texture;
            mono::PathDrawBuffer buffers;
        };

        CachedRiver CacheRiverData(uint32_t entity_id, const RiverComponent& component) const;
        bool NeedsUpdate(const CachedRiver& river, const RiverComponent& component) const;

        const RiverSystem* m_river_system;
        mono::PathSystem* m_path_system;
        const mono::TransformSystem* m_transform_system;
        uint32_t m_callback_id;

        mutable std::unordered_map<uint32_t, CachedRiver> m_cached_rivers;
    };
}
