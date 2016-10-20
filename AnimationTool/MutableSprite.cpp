
#include "MutableSprite.h"
#include "Sprite/ISprite.h"
#include "Sprite/SpriteFactory.h"
#include "IRenderer.h"

#include <algorithm>

using namespace animator;

MutableSprite::MutableSprite(const char* file)
{
    m_sprite = mono::CreateSprite(file);
    mPosition = math::Vector2f(100, 100);
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

void MutableSprite::SetAnimation(unsigned int id)
{
    const unsigned int animations = m_sprite->GetDefinedAnimations();
    if(id < animations)
        m_sprite->SetAnimation(id);
}

void MutableSprite::NextAnimation()
{
    int id = m_sprite->GetActiveAnimation();

    id++;

    id = std::min(id, m_sprite->GetDefinedAnimations() -1);
    m_sprite->SetAnimation(id);
}

void MutableSprite::PreviousAnimation()
{
    int id = m_sprite->GetActiveAnimation();
    id--;

    id = std::max(id, 0);
    m_sprite->SetAnimation(id);
}
