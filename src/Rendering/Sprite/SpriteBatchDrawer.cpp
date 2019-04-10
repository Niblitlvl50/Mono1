
#include "SpriteBatchDrawer.h"
#include "ISprite.h"
#include "Rendering/IRenderer.h"
#include "Rendering/Color.h"

#include "Math/Quad.h"
#include "Math/MathFunctions.h"
#include "SystemContext.h"

#include "TransformSystem.h"
#include "SpriteSystem.h"

using namespace mono;

namespace
{
    struct DrawContext
    {
        mono::IRenderer* renderer;
        mono::TransformSystem* transform_system;
    };

    struct SpriteTransformPair
    {
        math::Matrix transform;
        mono::ISprite* sprite;
    };

    struct CollectContext
    {
        mono::IRenderer* renderer;
        mono::TransformSystem* transform_system;
        std::vector<SpriteTransformPair>* sprites;
    };
}

SpriteBatchDrawer::SpriteBatchDrawer(SystemContext* system_context)
{
    m_transform_system = system_context->GetSystem<mono::TransformSystem>();
    m_sprite_system = system_context->GetSystem<mono::SpriteSystem>();
}

void SpriteBatchDrawer::doDraw(mono::IRenderer& renderer) const
{
    const auto collect_sprites = [](mono::ISprite* sprite, uint32_t id, void* context) {
        const CollectContext* collect_context = static_cast<const CollectContext*>(context);
        
        if(!sprite->GetTexture())
            return;

        const math::Quad world_bounds = collect_context->transform_system->GetWorldBoundingBox(id);
        if(collect_context->renderer->Cull(world_bounds))
        {
            const math::Matrix& transform = collect_context->transform_system->GetWorld(id);
            collect_context->sprites->push_back({ transform, sprite });
        }
    };

    std::vector<SpriteTransformPair> sprites_to_draw;

    CollectContext collect_context = {
        &renderer,
        m_transform_system,
        &sprites_to_draw
    };

    m_sprite_system->RunForEachSprite(collect_sprites, &collect_context);

    const auto sort_on_y = [](const SpriteTransformPair& first, const SpriteTransformPair& second) {
        const math::Vector& first_position = math::GetPosition(first.transform);
        const math::Vector& second_position = math::GetPosition(second.transform);
        return first_position.y > second_position.y;
    };

    std::sort(sprites_to_draw.begin(), sprites_to_draw.end(), sort_on_y);

    for(const SpriteTransformPair& sprite_transform : sprites_to_draw)
    {
        const math::Matrix world_transform = renderer.GetCurrentTransform() * sprite_transform.transform;

        renderer.PushNewTransform(world_transform);
        renderer.DrawSprite(*sprite_transform.sprite);
        renderer.PushGlobalTransform();
    }
}

math::Quad SpriteBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
