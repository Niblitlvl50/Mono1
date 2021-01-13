
#include "TextBatchDrawer.h"
#include "TextSystem.h"
#include "TransformSystem/TransformSystem.h"

#include "Math/Quad.h"
#include "Math/Matrix.h"

#include "Rendering/IRenderer.h"

using namespace mono;

TextBatchDrawer::TextBatchDrawer(mono::TextSystem* text_system, mono::TransformSystem* transform_system)
    : m_text_system(text_system)
    , m_transform_system(transform_system)
{ }

void TextBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    const auto draw_texts_func = [this, &renderer](mono::TextComponent& text, uint32_t index) {
        
        const math::Quad world_bb = m_transform_system->GetWorldBoundingBox(index);
        if(renderer.Cull(world_bb))
        {
            const math::Matrix& world_transform = m_transform_system->GetWorld(index);
            const math::Vector text_position = math::GetPosition(world_transform);
            if(text.draw_shadow)
            {
                const math::Vector shadow_position = text_position + math::Vector(0.1f, -0.05f);
                mono::Color::HSL hsl_color = mono::Color::ToHSL(text.tint);
                hsl_color.lightness -= 0.3f;

                renderer.RenderText(text.font_id, text.text.c_str(), shadow_position, text.centered, mono::Color::ToRGBA(hsl_color, text.tint.alpha));
            }

            renderer.RenderText(text.font_id, text.text.c_str(), text_position, text.centered, text.tint);
        }
    };

    m_text_system->ForEach(draw_texts_func);
}

math::Quad TextBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
