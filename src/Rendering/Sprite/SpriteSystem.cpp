
#include "SpriteSystem.h"
#include "Sprite.h"
#include "SpriteFactory.h"
#include "Rendering/RenderSystem.h"
#include "TransformSystem/TransformSystem.h"
#include "System/Hash.h"
#include "System/Debug.h"
#include "Util/Random.h"


using namespace mono;

SpriteSystem::SpriteSystem(uint32_t n_sprites, mono::TransformSystem* transform_system)
    : m_transform_system(transform_system)
    , m_sprites(n_sprites)
{
    m_enabled.resize(n_sprites, true);
}

mono::Sprite* SpriteSystem::AllocateSprite(uint32_t sprite_id)
{
    mono::Sprite* sprite = m_sprites.Set(sprite_id, mono::Sprite());

    m_enabled[sprite_id] = true;
    m_sprites_need_update.push_back(sprite_id);

    return sprite;
}

mono::ISprite* SpriteSystem::AllocateSprite(uint32_t sprite_id, const SpriteComponents& sprite_args)
{
    mono::Sprite* new_sprite = AllocateSprite(sprite_id);
    SetSpriteData(sprite_id, sprite_args);
    return new_sprite;
}

bool SpriteSystem::IsAllocated(uint32_t sprite_id)
{
    return m_sprites.IsActive(sprite_id);
}

void SpriteSystem::SetSpriteData(uint32_t sprite_id, const SpriteComponents& sprite_args)
{
    mono::Sprite* sprite = m_sprites.Get(sprite_id);
    mono::RenderSystem::GetSpriteFactory()->CreateSprite(*sprite, sprite_args.sprite_file);

    sprite->SetUserId(sprite_id);
    sprite->SetShade(sprite_args.shade);
    sprite->SetProperties(sprite_args.properties);
    sprite->SetShadowOffset(sprite_args.shadow_offset);
    sprite->SetShadowSize(sprite_args.shadow_size);

    if(sprite_args.animation_id >= 0 && sprite_args.animation_id < sprite->GetDefinedAnimations())
    {
        sprite->SetAnimation(sprite_args.animation_id);

        if(sprite_args.random_start_frame)
        {
            const size_t n_frames = sprite->GetSpriteData()->animations[sprite->GetActiveAnimation()].frames.size();
            const int random_frame = mono::RandomInt(0, n_frames -1);
            sprite->SetActiveAnimationFrame(random_frame);
        }
    }

    m_sprites_need_update.push_back(sprite_id);
}

void SpriteSystem::ReleaseSprite(uint32_t sprite_id)
{
    m_sprites.Release(sprite_id);
    //m_sprites[sprite_id].Init(nullptr, nullptr);
}

mono::Sprite* SpriteSystem::GetSprite(uint32_t sprite_id)
{
    return m_sprites.Get(sprite_id);
}

void SpriteSystem::SetSpriteEnabled(uint32_t sprite_id, bool enabled)
{
    m_enabled[sprite_id] = enabled;
}

const char* SpriteSystem::Name() const
{
    return "spritesystem";
}

void SpriteSystem::Update(const UpdateContext& update_context)
{
    const auto update_sprite = [this, &update_context](uint32_t index, mono::Sprite& sprite) {
        const bool is_enabled = m_enabled[index];
        if(is_enabled)
            sprite.Update(update_context);
    };
    m_sprites.ForEach(update_sprite);

    for(uint32_t sprite_id : m_sprites_need_update)
    {
        const mono::Sprite* sprite = m_sprites.Get(sprite_id);
        const mono::SpriteData* sprite_data = sprite->GetSpriteData();

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
    const auto proxy_func = [this, &func](uint32_t id, mono::Sprite& sprite)
    {
        const bool is_enabled = m_enabled[id];
        if(is_enabled)
            func(id, sprite);
    };
    m_sprites.ForEach(proxy_func);
}

uint32_t SpriteSystem::RunSpriteAnimSequence(uint32_t sprite_id, const std::vector<SpriteAnimNode>& anim_sequence)
{
    if(anim_sequence.empty())
        return 0;

    const SpriteAnimSequence sequence = { 0, anim_sequence };
    m_active_anim_sequences[sprite_id] = sequence;

    const SpriteAnimSequence& active_sequence = m_active_anim_sequences[sprite_id];
    const SpriteAnimNode& active_node = active_sequence.nodes[active_sequence.index];

    mono::Sprite* sprite = m_sprites.Get(sprite_id);
    sprite->SetAnimation(active_node.anim_name, std::bind(&SpriteSystem::AnimSequenceCallback, this, std::placeholders::_1));

    return 0;
}

void SpriteSystem::AnimSequenceCallback(uint32_t sprite_id)
{
    SpriteAnimSequence& active_sequence = m_active_anim_sequences[sprite_id];
    const SpriteAnimNode& active_node = active_sequence.nodes[active_sequence.index];
    active_node.callback(sprite_id);
    
    active_sequence.index++;

    if(active_sequence.index < (int)active_sequence.nodes.size())
    {
        const SpriteAnimNode& next_active_node = active_sequence.nodes[active_sequence.index];
        mono::Sprite* sprite = m_sprites.Get(sprite_id);
        sprite->SetAnimation(next_active_node.anim_name, std::bind(&SpriteSystem::AnimSequenceCallback, this, std::placeholders::_1));
    }
}
