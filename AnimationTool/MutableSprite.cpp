
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

void MutableSprite::SetAnimation(int id)
{
    const int animations = m_sprite->GetDefinedAnimations();
    if(id < animations)
        m_sprite->SetAnimation(id);
}

void MutableSprite::RestartAnimation()
{
    m_sprite->RestartAnimation();
}

int MutableSprite::NextAnimation() const
{
    int id = m_sprite->GetActiveAnimation();
    ++id;

    return std::min(id, m_sprite->GetDefinedAnimations() -1);
}

int MutableSprite::PreviousAnimation() const
{
    int id = m_sprite->GetActiveAnimation();
    --id;

    return std::max(id, 0);
}

int MutableSprite::CurrentAnimation() const
{
    return m_sprite->GetActiveAnimation();
}

const mono::AnimationSequence& MutableSprite::GetSequence(int id) const
{
    return m_sprite->GetSequence(id);
}

mono::AnimationSequence& MutableSprite::GetSequence(int id)
{
    return m_sprite->GetSequence(id);
}

const std::vector<mono::AnimationSequence>& MutableSprite::GetAnimations() const
{
    return m_sprite->GetAnimations();
}
