
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
        const math::Matrix& world_transform = m_transform_system->GetWorld(index);
        //renderer.Cull()
        renderer.DrawText(text.font_id, text.text.c_str(), math::GetPosition(world_transform), text.centered, text.tint);
    };
    m_text_system->ForEach(draw_texts_func);
}

math::Quad TextBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}
