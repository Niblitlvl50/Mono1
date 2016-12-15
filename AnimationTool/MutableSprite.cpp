
#include "MutableSprite.h"
#include "Sprite/ISprite.h"
#include "IRenderer.h"

using namespace animator;

MutableSprite::MutableSprite(mono::ISprite& sprite)
    : m_sprite(sprite)
{
    mScale = math::Vector(50, 50);
}

void MutableSprite::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(m_sprite);
}

void MutableSprite::Update(unsigned int delta)
{
    m_sprite.doUpdate(delta);
}