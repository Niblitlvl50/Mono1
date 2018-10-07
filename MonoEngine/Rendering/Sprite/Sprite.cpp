
#include "Sprite.h"
#include "Rendering/Texture/ITexture.h"

#include <stdexcept>
#include <cassert>
#include <cstring>

using namespace mono;

Sprite::Sprite()
{ }

Sprite::Sprite(const mono::ITexturePtr& texture, const std::vector<math::Quad>& coordinates)
{
    Init(texture, coordinates);
}

void Sprite::Init(const mono::ITexturePtr& texture, const std::vector<math::Quad>& coordinates)
{
    m_texture = texture;
    m_texture_coordinates = coordinates;
}

ITexturePtr Sprite::GetTexture() const
{
    return m_texture;
}

math::Quad Sprite::GetTextureCoords() const
{
    const AnimationSequence& anim = m_animations[m_active_animation];
    math::Quad coords = m_texture_coordinates.at(anim.Frame());

    if(m_flip_horizontal)
        std::swap(coords.mA.x, coords.mB.x);
    
    if(m_flip_vertical)
        std::swap(coords.mA.y, coords.mB.y);

    return coords;
}

const Color::RGBA& Sprite::GetShade() const
{
    return m_color;
}

void Sprite::SetShade(const mono::Color::RGBA& color)
{
    m_color = color;
}

void Sprite::SetHorizontalDirection(HorizontalDirection direction)
{
    m_flip_horizontal = (direction == HorizontalDirection::LEFT);
}

void Sprite::SetVerticalDirection(VerticalDirection direction)
{
    m_flip_vertical = (direction == VerticalDirection::DOWN);
}

void Sprite::doUpdate(unsigned int delta)
{
    AnimationSequence& anim = m_animations[m_active_animation];
    anim.Update(delta);

    if(anim.Finished() && m_callback)
        m_callback();
}

void Sprite::SetAnimation(int id)
{
    SetAnimation(id, nullptr);
}

void Sprite::SetAnimation(const char* name)
{
    SetAnimation(name, nullptr);
}

void Sprite::SetAnimation(const char* name, const std::function<void ()>& func)
{
    const int index = FindAnimationByName(name);
    assert(index != -1);

    SetAnimation(index, func);
}

void Sprite::SetAnimation(int id, const std::function<void ()>& func)
{
    assert(id < static_cast<int>(m_animations.size()));

    const bool same_id = (id == m_active_animation);
    m_active_animation = id;
    m_callback = func;

    if(!same_id)
        m_animations[id].Restart();
}

void Sprite::RestartAnimation()
{
    m_animations[m_active_animation].Restart();
}

int Sprite::DefineAnimation(const std::string& name, const std::vector<int>& frames, bool loop)
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
    return m_animations.size() -1;
}

int Sprite::GetDefinedAnimations() const
{
    return static_cast<int>(m_animations.size());
}

int Sprite::GetUniqueFrames() const
{
    return static_cast<int>(m_texture_coordinates.size());
}

const math::Quad& Sprite::GetFrame(int frame_index) const
{
    return m_texture_coordinates[frame_index];
}

int Sprite::GetActiveAnimation() const
{
    return m_active_animation;
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

int Sprite::FindAnimationByName(const char* name) const
{
    for(size_t index = 0; index < m_animations.size(); ++index)
    {
        const bool found = (std::strcmp(name, m_animations[index].GetName()) == 0);
        if(found)
            return index;
    }

    return -1;
}
