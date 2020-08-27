
#include "TextSystem.h"
#include "Util/Hash.h"

using namespace mono;

TextSystem::TextSystem(uint32_t n)
{
    m_texts.resize(n);
    m_alive.resize(n, false);
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
}

uint32_t TextSystem::Id() const
{
    return mono::Hash(Name());
}

const char* TextSystem::Name() const
{
    return "textsystem";
}

uint32_t TextSystem::Capacity() const
{
    return 0;
}

void TextSystem::Update(const mono::UpdateContext& update_context)
{
    // This should measure texts and set correct bounding box.
}
