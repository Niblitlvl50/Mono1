
#include "SpriteBatchDrawer.h"
#include "ISprite.h"
#include "Rendering/IRenderer.h"

#include "Math/Quad.h"
#include "SystemContext.h"

#include "TransformSystem/TransformSystem.h"
#include "SpriteSystem.h"

using namespace mono;

namespace
{
    struct SpriteTransformPair
    {
        math::Matrix transform;
        math::Quad world_bb;
        mono::ISprite* sprite;
        int layer;
    };
}

SpriteBatchDrawer::SpriteBatchDrawer(SystemContext* system_context)
{
    m_transform_system = system_context->GetSystem<mono::TransformSystem>();
    m_sprite_system = system_context->GetSystem<mono::SpriteSystem>();
}

void SpriteBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    std::vector<SpriteTransformPair> sprites_to_draw;
    sprites_to_draw.reserve(64);

    const auto collect_sprites = [this, &renderer, &sprites_to_draw](mono::ISprite* sprite, int layer, uint32_t id)
    {
        if(!sprite->GetTexture())
            return;

        const math::Quad& world_bounds = m_transform_system->GetWorldBoundingBox(id);
        if(renderer.Cull(world_bounds))
        {
            const math::Matrix& transform = m_transform_system->GetWorld(id);
            sprites_to_draw.push_back({ transform, world_bounds, sprite, layer });
        }
    };

    m_sprite_system->ForEachSprite(collect_sprites);

    const auto sort_on_y_and_layer = [](const SpriteTransformPair& first, const SpriteTransformPair& second)
    {
        if(first.layer == second.layer)
            return math::Bottom(first.world_bb) > math::Bottom(second.world_bb);

        return first.layer < second.layer;
    };

    std::sort(sprites_to_draw.begin(), sprites_to_draw.end(), sort_on_y_and_layer);

    for(const SpriteTransformPair& sprite_transform : sprites_to_draw)
    {
        const math::Matrix& world_transform = renderer.GetTransform() * sprite_transform.transform;
        auto transform_scope = mono::MakeTransformScope(world_transform, &renderer);
        renderer.DrawSprite(*sprite_transform.sprite);
    }
}

math::Quad SpriteBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
