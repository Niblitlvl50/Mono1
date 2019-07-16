
#include "SpriteSystem.h"
#include "Sprite.h"
#include "SpriteFactory.h"
#include "TransformSystem.h"
#include "Hash.h"

#include <cassert>

using namespace mono;

SpriteSystem::SpriteSystem(size_t n_sprites, mono::TransformSystem* transform_system)
    : m_transform_system(transform_system)
{
    m_sprites.resize(n_sprites);
    m_alive.resize(n_sprites, false);
}

SpriteSystem::~SpriteSystem()
{ }

mono::Sprite* SpriteSystem::AllocateSprite(uint32_t sprite_id)
{
    assert(!m_alive[sprite_id]);
    m_alive[sprite_id] = true;
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
    sprite.SetVerticalDirection(sprite_args.flip_vertical ? VerticalDirection::DOWN : VerticalDirection::UP);
    sprite.SetHorizontalDirection(sprite_args.flip_horizontal ? HorizontalDirection::LEFT : HorizontalDirection::RIGHT);

    if(sprite_args.animation_id >= 0 && sprite_args.animation_id < sprite.GetDefinedAnimations())
        sprite.SetAnimation(sprite_args.animation_id);
}

void SpriteSystem::ReleaseSprite(uint32_t sprite_id)
{
    m_alive[sprite_id] = false;
}

mono::Sprite* SpriteSystem::GetSprite(uint32_t sprite_id)
{
    return &m_sprites[sprite_id];
}

uint32_t SpriteSystem::Id() const
{
    return mono::Hash(Name());
}

const char* SpriteSystem::Name() const
{
    return "spritesystem";
}

uint32_t SpriteSystem::Capacity() const
{
    return m_sprites.size();
}

void SpriteSystem::Update(const UpdateContext& update_context)
{
    for(size_t index = 0; index < m_sprites.size(); ++index)
    {
        if(m_alive[index])
        {
            m_sprites[index].doUpdate(update_context);

            const mono::SpriteFrame sprite_frame = m_sprites[index].GetCurrentFrame();
            if(sprite_frame.size.x != 0.0f && sprite_frame.size.y != 0.0f)
            {
                const math::Vector& half_sprite_size = sprite_frame.size / 2.0f;

                math::Quad& bounding_box = m_transform_system->GetBoundingBox(index);
                bounding_box.mA = -half_sprite_size;
                bounding_box.mB = half_sprite_size;
            }
        }
    }
}

void SpriteSystem::ForEachSprite(ForEachSpriteFunc func)
{
    for(size_t index = 0; index < m_sprites.size(); ++index)
    {
        if(m_alive[index])
            func(&m_sprites[index], index);
    }
}
