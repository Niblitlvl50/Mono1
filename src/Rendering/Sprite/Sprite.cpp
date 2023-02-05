
#include "Sprite.h"
#include "SpriteProperties.h"
#include "Rendering/Texture/ITexture.h"

#include "System/System.h"
#include "System/Debug.h"

#include <stdexcept>
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
    m_sprite_user_id = -1;

    m_active_animation = 0;
    m_active_animation_done = false;
    m_active_frame = 0;
    m_active_frame_time = 0;
    m_playback_speed = 1.0f;
    m_playback_mode = PlaybackMode::PLAYING;

    m_properties = 0;
    m_color = mono::Color::RGBA();
    m_flash_timer = 0;
    m_callback = nullptr;
}

void Sprite::SetUserId(uint32_t id)
{
    m_sprite_user_id = id;
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

void Sprite::SetProperty(mono::SpriteProperty property)
{
    m_properties |= property;
}

bool Sprite::HasProperty(mono::SpriteProperty property) const
{
    return (m_properties & property);
}

void Sprite::ClearProperty(mono::SpriteProperty property)
{
    m_properties &= ~property;
}

void Sprite::Update(const UpdateContext& update_context)
{
    m_flash_timer -= 1;

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

    m_active_frame_time += (update_context.delta_s * 1000.0f) * m_playback_speed;
    if(m_active_frame_time > active_animation.frame_duration)
    {
        m_active_frame_time = 0; // Should we carry over the reminder from m_active_frame_time? (yes)

        const bool is_last_frame = (m_active_frame == int(active_animation.frames.size() - 1));
        if(is_last_frame)
        {
            if(active_animation.looping)
                m_active_frame = 0;
            else
                m_active_animation_done = true;

            if(m_callback)
                m_callback(m_sprite_user_id);
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
    MONO_ASSERT(id < static_cast<int>(m_sprite_data->animations.size()));
    MONO_ASSERT(id >= 0);

    m_active_animation = id;
    m_callback = callback;
    RestartAnimation();
}

void Sprite::SetAnimationPlaybackSpeed(float speed_scale)
{
    m_playback_speed = speed_scale;
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

    if(m_properties & mono::SpriteProperty::FLIP_HORIZONTAL)
    {
        std::swap(frame.uv_upper_left.x, frame.uv_lower_right.x);
        frame.center_offset.x = -frame.center_offset.x;
    }
    
    if(m_properties & mono::SpriteProperty::FLIP_VERTICAL)
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

void Sprite::FlashSprite()
{
    m_flash_timer = 3;
}

bool Sprite::ShouldFlashSprite() const
{
    return m_flash_timer > 0;
}

int Sprite::GetActiveAnimation() const
{
    return m_active_animation;
}

float Sprite::GetAnimationLengthSeconds(int animation_id) const
{
    const mono::SpriteAnimation& animation = m_sprite_data->animations[animation_id];
    return float(animation.frames.size() * animation.frame_duration) * 1000.0f;
}

int Sprite::GetAnimationIdFromName(const char* animation_name) const
{
    const auto find_by_name = [animation_name](const SpriteAnimation& animation) {
        return (std::strcmp(animation_name, animation.name.c_str()) == 0);
    };

    const auto it = std::find_if(m_sprite_data->animations.begin(), m_sprite_data->animations.end(), find_by_name);
    if(it != m_sprite_data->animations.end())
        return std::distance(m_sprite_data->animations.begin(), it);

    System::Log("sprite|Unable to find animation with name '%s'", animation_name);
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
