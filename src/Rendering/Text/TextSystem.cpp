
#include "TextSystem.h"
#include "TextFunctions.h"

#include "Math/Vector.h"
#include "Math/Quad.h"
#include "Math/Matrix.h"

#include "TransformSystem/TransformSystem.h"
#include "Util/Hash.h"

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

uint32_t TextSystem::Id() const
{
    return mono::Hash(Name());
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

        const math::Vector size = mono::MeasureString(text.font_id, text.text.c_str());
        
        math::Quad& bounding_box = m_transform_system->GetBoundingBox(index);
        bounding_box = math::Quad(math::ZeroVec, size);

        m_text_dirty[index] = false;
    };

    ForEach(update_bb);
}
