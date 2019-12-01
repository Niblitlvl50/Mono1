
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
        mono::ISprite* sprite;
    };
}

SpriteBatchDrawer::SpriteBatchDrawer(SystemContext* system_context)
{
    m_transform_system = system_context->GetSystem<mono::TransformSystem>();
    m_sprite_system = system_context->GetSystem<mono::SpriteSystem>();
}

void SpriteBatchDrawer::doDraw(mono::IRenderer& renderer) const
{
    std::vector<SpriteTransformPair> sprites_to_draw;

    const auto collect_sprites = [this, &renderer, &sprites_to_draw](mono::ISprite* sprite, uint32_t id) {
        if(!sprite->GetTexture())
            return;

        const math::Quad& world_bounds = m_transform_system->GetWorldBoundingBox(id);
        if(renderer.Cull(world_bounds))
        {
            const math::Matrix& transform = m_transform_system->GetWorld(id);
            sprites_to_draw.push_back({ transform, sprite });
        }
    };

    m_sprite_system->ForEachSprite(collect_sprites);

    const auto sort_on_y = [](const SpriteTransformPair& first, const SpriteTransformPair& second) {
        const math::Vector& first_position = math::GetPosition(first.transform);
        const math::Vector& second_position = math::GetPosition(second.transform);
        return first_position.y > second_position.y;
    };

    std::sort(sprites_to_draw.begin(), sprites_to_draw.end(), sort_on_y);

    for(const SpriteTransformPair& sprite_transform : sprites_to_draw)
    {
        const math::Matrix& world_transform = renderer.GetCurrentTransform() * sprite_transform.transform;

        renderer.PushNewTransform(world_transform);
        renderer.DrawSprite(*sprite_transform.sprite);
        renderer.PushGlobalTransform();
    }
}

math::Quad SpriteBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
