
#include "TextBatchDrawer.h"
#include "TextSystem.h"
#include "TextFunctions.h"
#include "TextBufferFactory.h"
#include "TransformSystem/TransformSystem.h"

#include "Math/Quad.h"
#include "Math/Matrix.h"

#include "Rendering/IRenderer.h"
#include "Rendering/RenderBuffer/IRenderBuffer.h"
#include "Rendering/RenderBuffer/BufferFactory.h"

using namespace mono;

TextBatchDrawer::TextBatchDrawer(mono::TextSystem* text_system, mono::TransformSystem* transform_system)
    : m_text_system(text_system)
    , m_transform_system(transform_system)
{
    m_draw_buffers.resize(500);
}

TextBatchDrawer::~TextBatchDrawer()
{ }

void TextBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    const auto draw_texts_func = [this, &renderer](mono::TextComponent& text, uint32_t index) {
        
        const math::Quad world_bb = m_transform_system->GetWorldBoundingBox(index);
        if(renderer.Cull(world_bb))
        {
            const math::Matrix& world_transform = m_transform_system->GetWorld(index);
            auto transform_scope = mono::MakeTransformScope(world_transform, &renderer);

            const TextDrawBuffers* render_buffers = UpdateDrawBuffers(text, index);
            const ITexturePtr texture = mono::GetFontTexture(text.font_id);

            if(text.text.empty())
                return;

            if(text.draw_shadow)
            {
                math::Matrix shadow_world_transform = world_transform;
                const math::Vector shadow_offset(0.1f, -0.05f);
                math::Translate(shadow_world_transform, shadow_offset);

                auto shadow_transform_scope = mono::MakeTransformScope(shadow_world_transform, &renderer);

                mono::Color::HSL hsl_color = mono::Color::ToHSL(text.tint);
                hsl_color.lightness -= 0.3f;

                renderer.RenderText(
                    render_buffers->vertices.get(),
                    render_buffers->uv.get(),
                    render_buffers->indices.get(),
                    texture.get(),
                    mono::Color::ToRGBA(hsl_color, text.tint.alpha));
            }

            renderer.RenderText(
                render_buffers->vertices.get(),
                render_buffers->uv.get(),
                render_buffers->indices.get(),
                texture.get(),
                text.tint);
        }
    };

    m_text_system->ForEach(draw_texts_func);
}

math::Quad TextBatchDrawer::BoundingBox() const
{
    return math::InfQuad;
}

const mono::TextDrawBuffers* TextBatchDrawer::UpdateDrawBuffers(const mono::TextComponent& text_component, uint32_t index) const
{
    bool needs_update = true;

    auto it = m_current_data.find(index);
    if(it != m_current_data.end())
    {
        const mono::TextComponent& last_text_update = it->second;
        needs_update =
            last_text_update.font_id != text_component.font_id ||
            last_text_update.center_flags != text_component.center_flags ||
            last_text_update.text != text_component.text;
    }

    TextDrawBuffers& draw_buffers = m_draw_buffers[index];

    if(needs_update && !text_component.text.empty())
    {
        draw_buffers =
            mono::BuildTextDrawBuffers(text_component.font_id, text_component.text.c_str(), text_component.center_flags);
        m_current_data[index] = text_component;
    }

    return &draw_buffers;
}
