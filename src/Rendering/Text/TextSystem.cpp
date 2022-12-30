
#include "TextSystem.h"
#include "TextFunctions.h"

#include "Math/Vector.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"

#include "TransformSystem/TransformSystem.h"
#include "System/Hash.h"

using namespace mono;

TextSystem::TextSystem(uint32_t n, mono::TransformSystem* transform_system)
    : m_transform_system(transform_system)
{
    m_texts.resize(n);
    m_alive.resize(n, false);
    m_text_dirty.resize(n, true);
}

mono::TextComponent* TextSystem::AllocateText(uint32_t id)
{
    m_alive[id] = true;
    return &m_texts[id];
}

void TextSystem::ReleaseText(uint32_t id)
{
    m_alive[id] = false;
}

void TextSystem::SetTextData(uint32_t id, const mono::TextComponent& text_data)
{
    m_texts[id] = text_data;
    m_text_dirty[id] = true;
}

const char* TextSystem::Name() const
{
    return "textsystem";
}

void TextSystem::Update(const mono::UpdateContext& update_context)
{
    const auto update_bb = [this](const TextComponent& text, uint32_t index) {
        if(!m_text_dirty[index])
            return;

        math::Vector text_offset;
        const math::Vector text_size = mono::MeasureString(text.font_id, text.text.c_str());
        const math::Vector half_size = text_size / 2.0f;

        if(text.center_flags & FontCentering::HORIZONTAL)
            text_offset.x = half_size.x;
        if(text.center_flags & FontCentering::VERTICAL)
            text_offset.y = half_size.y;

        math::Quad& bounding_box = m_transform_system->GetBoundingBox(index);
        bounding_box = math::Quad(-text_offset, text_size - text_offset);

        m_text_dirty[index] = false;
    };

    ForEach(update_bb);
}
