//
//  AnimatedSprite.cpp
//  Mono1
//
//  Created by Niblit on 2012-04-09.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "Sprite.h"
#include "SpriteAttributes.h"
#include "Texture/ITexture.h"

#include <stdexcept>

using namespace mono;

namespace
{
    constexpr int DEFAULT_ANIMATION_ID = 0;
}

Sprite::Sprite(const mono::ITexturePtr& texture, const std::vector<math::Quad>& coordinates)
    : m_activeAnimationId(DEFAULT_ANIMATION_ID),
      m_texture(texture),
      m_textureCoordinates(coordinates)
{ }

ITexturePtr Sprite::GetTexture() const
{
    return m_texture;
}

const math::Quad& Sprite::GetTextureCoords() const
{
    const AnimationSequence& anim = m_definedAnimations.find(m_activeAnimationId)->second;
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
    AnimationSequence& anim = m_definedAnimations.find(m_activeAnimationId)->second;
    anim.Tick(delta);

    if(anim.Finished() && m_callbackFunction)
        m_callbackFunction();
}

void Sprite::SetAnimation(int id)
{
    SetAnimation(id, nullptr);
}

void Sprite::SetAnimation(int id, const std::function<void ()>& func)
{
    m_activeAnimationId = id;
    m_callbackFunction = func;

    m_definedAnimations.find(id)->second.Restart();
}

void Sprite::DefineAnimation(int id, const std::vector<int>& frames, bool loop)
{
    const bool even = (frames.size() % 2) == 0;
    if(!even)
        throw std::runtime_error("Animation vector does not match up, not an even number of values");

    AnimationSequence sequence(loop);

    for(auto it = frames.begin(), end = frames.end(); it != end; ++it)
    {
        const unsigned int frame = *it;
        ++it;
        const unsigned int duration = *it;

        sequence.AddFrame(frame, duration);
    }

    m_definedAnimations.insert(std::make_pair(id, sequence));
}

void Sprite::DefineAnimation(int id, const std::vector<int>& frames, const std::vector<std::string>& attributes)
{
    const bool noloop = std::find_if(attributes.begin(), attributes.end(), FindNoLoopAttribute()) != attributes.end();
    DefineAnimation(id, frames, !noloop);
}

int Sprite::GetDefinedAnimations() const
{
    return static_cast<int>(m_definedAnimations.size());
}

int Sprite::GetActiveAnimation() const
{
    return m_activeAnimationId;
}



