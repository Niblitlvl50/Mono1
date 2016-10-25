
#include "MutableSprite.h"
#include "Sprite/ISprite.h"
#include "IRenderer.h"

using namespace animator;

MutableSprite::MutableSprite(const mono::ISpritePtr& sprite)
    : m_sprite(sprite)
{
    mScale = math::Vector2f(50, 50);
}

void MutableSprite::Draw(mono::IRenderer& renderer) const
{
    renderer.DrawSprite(*m_sprite);
}

void MutableSprite::Update(unsigned int delta)
{
    m_sprite->doUpdate(delta);
}