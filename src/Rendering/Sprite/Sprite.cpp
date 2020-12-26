
#include "Sprite.h"
#include "Rendering/Texture/ITexture.h"

#include <stdexcept>
#include <cassert>
#include <cstring>

using namespace mono;

Sprite::Sprite()
    : Sprite(nullptr, nullptr)
{ }

Sprite::Sprite(const SpriteData* sprite_data, mono::ITexturePtr texture)
{
    Init(sprite_data, texture);
}

void Sprite::Init(const SpriteData* sprite_data, mono::ITexturePtr texture)
{
    m_sprite_data = sprite_data;
    m_texture = texture;

    m_active_animation = 0;
    m_active_animation_done = false;
    m_active_frame = 0;
    m_active_frame_time = 0;
    m_playback_mode = PlaybackMode::PLAYING;

    m_properties = 0;
    m_flip_horizontal = false;
    m_flip_vertical = false;
    m_color = mono::Color::RGBA();
    m_callback = nullptr;
}

uint32_t Sprite::GetSpriteHash() const
{
    return m_sprite_data != nullptr ? m_sprite_data->hash : 0;
}

ITexture* Sprite::GetTexture() const
{
    return m_texture.get();
}

mono::SpriteFrame Sprite::GetCurrentFrame() const
{
    if(!m_sprite_data || m_sprite_data->animations.empty())
        return mono::SpriteFrame();

    return GetFrame(GetCurrentFrameIndex());
}

int Sprite::GetCurrentFrameIndex() const
{
    const SpriteAnimation& animation = m_sprite_data->animations[m_active_animation];
    return  animation.frames[m_active_frame];
}

const Color::RGBA& Sprite::GetShade() const
{
    return m_color;
}

void Sprite::SetShade(const mono::Color::RGBA& color)
{
    m_color = color;
}

math::Vector Sprite::GetShadowOffset() const
{
    return m_shadow_offset;
}

void Sprite::SetShadowOffset(const math::Vector& shadow_offset)
{
    m_shadow_offset = shadow_offset;
}

uint32_t Sprite::GetProperties() const
{
    return m_properties;
}

void Sprite::SetProperties(uint32_t properties)
{
    m_properties = properties;
}

void Sprite::SetHorizontalDirection(HorizontalDirection direction)
{
    m_flip_horizontal = (direction == HorizontalDirection::LEFT);
}

mono::HorizontalDirection Sprite::GetHorizontalDirection() const
{
    return m_flip_horizontal ? mono::HorizontalDirection::LEFT : mono::HorizontalDirection::RIGHT;
}

void Sprite::SetVerticalDirection(VerticalDirection direction)
{
    m_flip_vertical = (direction == VerticalDirection::DOWN);
}

mono::VerticalDirection Sprite::GetVerticalDirection() const
{
    return m_flip_vertical ? mono::VerticalDirection::DOWN : mono::VerticalDirection::UP;
}

void Sprite::Update(const UpdateContext& update_context)
{
    if(m_active_animation_done || m_playback_mode == PlaybackMode::PAUSED)
        return;

    if(!m_sprite_data)
        return;

    const std::vector<SpriteAnimation>& animations = m_sprite_data->animations;
    if(animations.empty())
        return;

    const SpriteAnimation& active_animation = animations[m_active_animation];
    if(active_animation.frames.empty())
        return;

    m_active_frame_time += update_context.delta_ms;

    if(m_active_frame_time > active_animation.frame_duration)
    {
        m_active_frame_time = 0; // Should we carry over the reminder from m_active_frame_time? (yes)

        const bool is_last_frame = (m_active_frame == int(active_animation.frames.size() - 1));
        if(is_last_frame)
        {
            if(m_callback)
                m_callback();

            if(active_animation.looping)
                m_active_frame = 0;
            else
                m_active_animation_done = true;
        }
        else
        {
            m_active_frame++;
        }
    }
}

void Sprite::SetAnimation(int id)
{
    SetAnimation(id, nullptr);
}

void Sprite::SetAnimation(const char* name)
{
    SetAnimation(name, nullptr);
}

void Sprite::SetAnimation(const char* animation_name, const SpriteAnimationCallback& callback)
{
    const int animation_id = GetAnimationIdFromName(animation_name);
    SetAnimation(animation_id, callback);
}

void Sprite::SetAnimation(int id, const SpriteAnimationCallback& callback)
{
    assert(id < static_cast<int>(m_sprite_data->animations.size()));

    const bool same_id = (id == m_active_animation);

    m_active_animation = id;
    m_callback = callback;

    if(!same_id)
        RestartAnimation();
}

void Sprite::SetAnimationPlayback(PlaybackMode mode)
{
    m_playback_mode = mode;
}

void Sprite::RestartAnimation()
{
    m_active_frame = 0;
    m_active_frame_time = 0;
    m_active_animation_done = false;
}

int Sprite::GetDefinedAnimations() const
{
    if(m_sprite_data)
        return static_cast<int>(m_sprite_data->animations.size());

    return 0;
}

mono::SpriteFrame Sprite::GetFrame(int frame_index) const
{
    SpriteFrame frame = m_sprite_data->frames[frame_index];

    if(m_flip_horizontal)
    {
        std::swap(frame.uv_upper_left.x, frame.uv_lower_right.x);
        frame.center_offset.x = -frame.center_offset.x;
    }
    
    if(m_flip_vertical)
    {
        std::swap(frame.uv_lower_right.y, frame.uv_upper_left.y);
        frame.center_offset.y = -frame.center_offset.y;
    }

    return frame;
}

const mono::SpriteData* Sprite::GetSpriteData() const
{
    return m_sprite_data;
}

void Sprite::SetShadowSize(float size)
{
    m_shadow_size = size;
}

float Sprite::GetShadowSize() const
{
    return m_shadow_size;
}

int Sprite::GetActiveAnimation() const
{
    return m_active_animation;
}

uint32_t Sprite::GetAnimationLength(int animation_id) const
{
    const mono::SpriteAnimation& animation = m_sprite_data->animations[animation_id];
    return (animation.frames.size() * animation.frame_duration);
}

uint32_t Sprite::GetAnimationLength(const char* animation_name) const
{
    const int animation_id = GetAnimationIdFromName(animation_name);
    return GetAnimationLength(animation_id);
}

int Sprite::GetAnimationIdFromName(const char* animation_name) const
{
    const auto find_by_name = [animation_name](const SpriteAnimation& animation) {
        return (std::strcmp(animation_name, animation.name.c_str()) == 0);
    };

    const auto it = std::find_if(m_sprite_data->animations.begin(), m_sprite_data->animations.end(), find_by_name);
    if(it != m_sprite_data->animations.end())
        return std::distance(m_sprite_data->animations.begin(), it);

    return -1;
}

void Sprite::SetActiveAnimationFrame(int frame)
{
    m_active_frame = frame;
}

int Sprite::GetActiveAnimationFrame() const
{
    return m_active_frame;
}

bool Sprite::IsActiveAnimationDone() const
{
    return m_active_animation_done;
}
