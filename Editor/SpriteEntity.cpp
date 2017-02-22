
#include "SpriteEntity.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "IRenderer.h"
#include "Color.h"
#include "Math/Quad.h"

using namespace editor;

SpriteEntity::SpriteEntity(const char* file)
    : m_selected(false)
{
    m_sprite = mono::CreateSprite(file);
}

void SpriteEntity::Draw(mono::IRenderer& renderer) const
{
    constexpr mono::Color::RGBA selected_color(0.0f, 1.0f, 0.0f);

    renderer.DrawSprite(*m_sprite);

    if(m_selected)
    {
        math::Quad bb = BoundingBox();
        bb.mA -= Position();
        bb.mB -= Position();
        renderer.DrawQuad(bb, selected_color, 2.0f);
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
