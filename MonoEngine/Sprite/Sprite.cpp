//
//  AnimatedSprite.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Sprite.h"
#include "Texture/ITexture.h"

#include <stdexcept>

using namespace mono;

Sprite::Sprite()
    : m_activeAnimation(0)
{ }

Sprite::Sprite(const mono::ITexturePtr& texture, const std::vector<math::Quad>& coordinates)
    : m_activeAnimation(0),
      m_texture(texture),
      m_textureCoordinates(coordinates)
{ }

void Sprite::Init(const mono::ITexturePtr& texture, const std::vector<math::Quad>& coordinates)
{
      m_texture = texture;
      m_textureCoordinates = coordinates;
}

ITexturePtr Sprite::GetTexture() const
{
    return m_texture;
}

const math::Quad& Sprite::GetTextureCoords() const
{
    const AnimationSequence& anim = m_animations[m_activeAnimation];
    return anim.Finished() ? math::zeroQuad : m_textureCoordinates.at(anim.Frame());
}

const Color::RGBA& Sprite::GetShade() const
{
    return m_color;
}

void Sprite::SetShade(const mono::Color::RGBA& color)
{
    m_color = color;
}

void Sprite::doUpdate(unsigned int delta)
{
    AnimationSequence& anim = m_animations[m_activeAnimation];
    anim.Update(delta);

    if(anim.Finished() && m_callback)
        m_callback();
}

void Sprite::SetAnimation(int id)
{
    SetAnimation(id, nullptr);
}

void Sprite::SetAnimation(int id, const std::function<void ()>& func)
{
    const bool same_id = (id == m_activeAnimation);

    m_activeAnimation = id;
    m_callback = func;

    if(!same_id)
        m_animations[id].Restart();
}

void Sprite::RestartAnimation()
{
    m_animations[m_activeAnimation].Restart();
}

void Sprite::DefineAnimation(const std::string& name, const std::vector<int>& frames, bool loop)
{
    const bool even = (frames.size() % 2) == 0;
    if(!even)
        throw std::runtime_error("Animation vector does not match up, not an even number of values");

    AnimationSequence sequence(name.c_str(), loop);

    for(auto it = frames.begin(), end = frames.end(); it != end; ++it)
    {
        const int frame = *it;
        ++it;
        const int duration = *it;

        sequence.AddFrame(frame, duration);
    }

    m_animations.push_back(sequence);
}

int Sprite::GetDefinedAnimations() const
{
    return static_cast<int>(m_animations.size());
}

int Sprite::GetUniqueFrames() const
{
    return static_cast<int>(m_textureCoordinates.size());
}

int Sprite::GetActiveAnimation() const
{
    return m_activeAnimation;
}

const AnimationSequence& Sprite::GetSequence(int id) const
{
    return m_animations[id];
}

AnimationSequence& Sprite::GetSequence(int id)
{
    return m_animations[id];
}

const std::vector<AnimationSequence>& Sprite::GetAnimations() const
{
    return m_animations;
}

std::vector<AnimationSequence>& Sprite::GetAnimations()
{
    return m_animations;
}
