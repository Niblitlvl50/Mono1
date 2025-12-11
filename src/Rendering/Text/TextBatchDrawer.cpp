
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

#include <algorithm>
#include <iterator>

using namespace mono;

TextBatchDrawer::TextBatchDrawer(mono::TextSystem* text_system, mono::TransformSystem* transform_system)
    : m_text_system(text_system)
    , m_transform_system(transform_system)
{
    m_draw_buffers.resize(1000);
}

TextBatchDrawer::~TextBatchDrawer()
{ }

void TextBatchDrawer::Draw(mono::IRenderer& renderer) const
{
    std::vector<uint32_t> active_entities;

    const auto draw_texts_func = [this, &renderer, &active_entities](mono::TextComponent& text, uint32_t index) {
        
        if(text.text.empty())
            return;

        const math::Quad world_bb = m_transform_system->GetWorldBoundingBox(index);
        if(renderer.Cull(world_bb) == mono::CullResult::OUTSIDE_VIEW)
            return;

        active_entities.push_back(index);

        const math::Matrix& world_transform = m_transform_system->GetWorld(index);
        const TextDrawBuffers* render_buffers = UpdateDrawBuffers(text, index);
        const ITexturePtr texture = mono::GetFontTexture(text.font_id);

        if(text.draw_shadow)
        {
            math::Matrix shadow_world_transform = world_transform;
            math::Translate(shadow_world_transform, text.shadow_offset);

            auto shadow_transform_scope = mono::MakeTransformScope(shadow_world_transform, &renderer);
            renderer.DrawGeometry(
                render_buffers->vertices.get(),
                render_buffers->uv.get(),
                render_buffers->indices.get(),
                texture.get(),
                text.shadow_color,
                false,
                render_buffers->indices->Size());
        }

        auto transform_scope = mono::MakeTransformScope(world_transform, &renderer);
        renderer.DrawGeometry(
            render_buffers->vertices.get(),
            render_buffers->uv.get(),
            render_buffers->indices.get(),
            texture.get(),
            text.tint,
            false,
            render_buffers->indices->Size());
    };

    m_text_system->ForEach(draw_texts_func);


    std::vector<uint32_t> diff_result;
    std::set_difference(
        m_last_active_entities.begin(),
        m_last_active_entities.end(),
        active_entities.begin(),
        active_entities.end(),
        std::back_inserter(diff_result));

    m_last_active_entities = active_entities;
    
    for(uint32_t id : diff_result)
    {
        m_draw_buffers[id] = mono::TextDrawBuffers();
        m_current_data.erase(id);
    }
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
