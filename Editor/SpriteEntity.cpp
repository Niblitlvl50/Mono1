
#include "SpriteEntity.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "IRenderer.h"
#include "Color.h"
#include "Math/Quad.h"

using namespace editor;

SpriteEntity::SpriteEntity(const char* name, const char* file)
    : m_name(name),
      m_selected(false)
{
    m_sprite = mono::CreateSprite(file);
}

void SpriteEntity::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*m_sprite);

    if(m_selected)
    {
        math::Quad bb(-0.5f, -0.5f, 0.5f, 0.5f);
        renderer.DrawQuad(bb, mono::Color::RGBA(0.0f, 1.0f, 0.0f), 2.0f);
    }
}

void SpriteEntity::Update(unsigned int delta)
{
    m_sprite->doUpdate(delta);
}

void SpriteEntity::SetSelected(bool selected)
{
    m_selected = selected;
}

const std::string& SpriteEntity::Name() const
{
    return m_name;
}
