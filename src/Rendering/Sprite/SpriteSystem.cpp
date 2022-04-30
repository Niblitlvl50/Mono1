
#include "SpriteSystem.h"
#include "Sprite.h"
#include "SpriteFactory.h"
#include "Rendering/RenderSystem.h"
#include "TransformSystem/TransformSystem.h"
#include "System/Hash.h"
#include "Util/Random.h"

#include <cassert>

using namespace mono;

SpriteSystem::SpriteSystem(uint32_t n_sprites, mono::TransformSystem* transform_system)
    : m_transform_system(transform_system)
{
    m_sprites.resize(n_sprites);
    m_sprite_layers.resize(n_sprites, 0);
    m_sprite_sort_offsets.resize(n_sprites, 0.0f);
    m_enabled.resize(n_sprites, true);
    m_alive.resize(n_sprites, false);
}

mono::Sprite* SpriteSystem::AllocateSprite(uint32_t sprite_id)
{
    assert(!m_alive[sprite_id]);
    m_alive[sprite_id] = true;
    m_sprite_layers[sprite_id] = 0;
    m_enabled[sprite_id] = true;

    m_sprites_need_update.push_back(sprite_id);

    return &m_sprites[sprite_id];
}

mono::ISprite* SpriteSystem::AllocateSprite(uint32_t sprite_id, const SpriteComponents& sprite_args)
{
    mono::Sprite* new_sprite = AllocateSprite(sprite_id);
    SetSpriteData(sprite_id, sprite_args);
    return new_sprite;
}

bool SpriteSystem::IsAllocated(uint32_t sprite_id)
{
    return m_alive[sprite_id];
}

void SpriteSystem::SetSpriteData(uint32_t sprite_id, const SpriteComponents& sprite_args)
{
    assert(m_alive[sprite_id]);

    mono::Sprite& sprite = m_sprites[sprite_id];
    mono::GetSpriteFactory()->CreateSprite(sprite, sprite_args.sprite_file);

    sprite.SetShade(sprite_args.shade);
    sprite.SetProperties(sprite_args.properties);
    sprite.SetShadowOffset(sprite_args.shadow_offset);
    sprite.SetShadowSize(sprite_args.shadow_size);

    if(sprite_args.animation_id >= 0 && sprite_args.animation_id < sprite.GetDefinedAnimations())
    {
        sprite.SetAnimation(sprite_args.animation_id);

        if(sprite_args.random_start_frame)
        {
            const size_t n_frames = sprite.GetSpriteData()->animations[sprite.GetActiveAnimation()].frames.size();
            const int random_frame = mono::RandomInt(0, n_frames -1);
            sprite.SetActiveAnimationFrame(random_frame);
        }
    }

    m_sprite_layers[sprite_id] = sprite_args.layer;
    m_sprite_sort_offsets[sprite_id] = sprite_args.sort_offset;

    m_sprites_need_update.push_back(sprite_id);
}

void SpriteSystem::ReleaseSprite(uint32_t sprite_id)
{
    m_alive[sprite_id] = false;
    m_sprites[sprite_id].Init(nullptr, nullptr);
}

mono::Sprite* SpriteSystem::GetSprite(uint32_t sprite_id)
{
    return &m_sprites[sprite_id];
}

int SpriteSystem::GetSpriteLayer(uint32_t sprite_id) const
{
    return m_sprite_layers[sprite_id];
}

void SpriteSystem::SetSpriteLayer(uint32_t sprite_id, int new_layer)
{
    m_sprite_layers[sprite_id] = new_layer;
}

float SpriteSystem::GetSpriteSortOffset(uint32_t sprite_id) const
{
    return m_sprite_sort_offsets[sprite_id];
}

void SpriteSystem::SetSpriteEnabled(uint32_t sprite_id, bool enabled)
{
    m_enabled[sprite_id] = enabled;
}

uint32_t SpriteSystem::Id() const
{
    return hash::Hash(Name());
}

const char* SpriteSystem::Name() const
{
    return "spritesystem";
}

void SpriteSystem::Update(const UpdateContext& update_context)
{
    for(size_t index = 0; index < m_sprites.size(); ++index)
    {
        if(!m_alive[index] || !m_enabled[index])
            continue;

        mono::Sprite& sprite = m_sprites[index];
        sprite.Update(update_context);
    }

    for(uint32_t sprite_id : m_sprites_need_update)
    {
        const mono::Sprite& sprite = m_sprites[sprite_id];
        const mono::SpriteData* sprite_data = sprite.GetSpriteData();

        if(!sprite_data)
            continue;

        math::Vector frame_size;

        for(const mono::SpriteFrame& frame : sprite_data->frames)
        {
            frame_size.x = std::max(frame_size.x, frame.size.x);
            frame_size.y = std::max(frame_size.y, frame.size.y);
        }

        if(frame_size.x != 0.0f && frame_size.y != 0.0f)
        {
            const math::Vector& half_sprite_size = frame_size / 2.0f;

            math::Quad& bounding_box = m_transform_system->GetBoundingBox(sprite_id);
            bounding_box.bottom_left = -half_sprite_size;
            bounding_box.top_right = half_sprite_size;
        }
    }

    m_sprites_need_update.clear();
}

void SpriteSystem::ForEachSprite(const ForEachSpriteFunc& func)
{
    for(uint32_t index = 0; index < m_sprites.size(); ++index)
    {
        if(m_alive[index] && m_enabled[index])
            func(&m_sprites[index], m_sprite_layers[index], index);
    }
}
