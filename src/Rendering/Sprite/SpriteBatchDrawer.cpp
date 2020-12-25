
#include "SpriteBatchDrawer.h"

#include "ISprite.h"
#include "SpriteSystem.h"
#include "SpriteProperties.h"
#include "Rendering/IRenderer.h"
#include "TransformSystem/TransformSystem.h"

#include "Math/Quad.h"

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

SpriteBatchDrawer::SpriteBatchDrawer(const mono::TransformSystem* transform_system, mono::SpriteSystem* sprite_system)
    : m_transform_system(transform_system)
    , m_sprite_system(sprite_system)
{ }

SpriteBatchDrawer::~SpriteBatchDrawer()
{ }

void SpriteBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    std::vector<SpriteTransformPair> sprites_to_draw;
    sprites_to_draw.reserve(128);

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

        const uint32_t sprite_properties = sprite_transform.sprite->GetProperties();
        if(sprite_properties & mono::SpriteProperty::SHADOW)
        {
            const math::Vector shadow_offset = sprite_transform.sprite->GetShadowOffset();
            const float shadow_size = sprite_transform.sprite->GetShadowSize();
            constexpr mono::Color::RGBA shadow_color(0.2f, 0.2f, 0.2f, 0.5f);
            renderer.DrawFilledCircle(shadow_offset, math::Vector(shadow_size, shadow_size / 2.0f), 8, shadow_color);
        }

        renderer.DrawSprite(*sprite_transform.sprite);
    }
}

math::Quad SpriteBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
