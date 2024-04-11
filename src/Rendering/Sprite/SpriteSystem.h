
#pragma once

#include "IGameSystem.h"
#include "Math/Vector.h"
#include "Rendering/Color.h"
#include "Util/ActiveVector.h"
#include "ISprite.h"

#include <vector>
#include <functional>
#include <unordered_map>
#include <unordered_set>

namespace mono
{
    class Sprite;
    class TransformSystem;

    struct SpriteComponents
    {
        const char* sprite_file = nullptr;
        mono::Color::RGBA shade;
        bool random_start_frame = false;
        int animation_id = 0;
        int layer = 0;
        float sort_offset = 0.0f;
        uint32_t properties = 0;
        math::Vector shadow_offset;
        float shadow_size;
    };

    struct SpriteAnimNode
    {
        const char* anim_name = nullptr;
        mono::SpriteAnimationCallback callback;
    };

    struct SpriteAnimSequence
    {
        int index;
        std::vector<SpriteAnimNode> nodes;
    };

    using ForEachSpriteFunc = std::function<void (uint32_t sprite_id, mono::ISprite& sprite)>;

    class SpriteSystem : public mono::IGameSystem
    {
    public:

        SpriteSystem(uint32_t n_sprites, mono::TransformSystem* transform_system);

        mono::ISprite* AllocateSprite(uint32_t sprite_id);
        mono::ISprite* AllocateSprite(uint32_t sprite_id, const SpriteComponents& sprite_args);
        bool IsAllocated(uint32_t sprite_id);
        void SetSpriteData(uint32_t sprite_id, const SpriteComponents& component_data);
        void ReleaseSprite(uint32_t sprite_id);

        mono::Sprite* GetSprite(uint32_t sprite_id);
        void SetSpriteFile(uint32_t sprite_id, const char* sprite_file);
        void SetSpriteEnabled(uint32_t sprite_id, bool enabled);
        void ForEachSprite(const ForEachSpriteFunc& func);

        uint32_t RunSpriteAnimSequence(uint32_t sprite_id, const std::vector<SpriteAnimNode>& anim_sequence);
        void AnimSequenceCallback(uint32_t sprite_id);

        const char* Name() const override;
        void Update(const UpdateContext& update_context) override;

    private:

        mono::TransformSystem* m_transform_system;
        mono::ActiveVector<mono::Sprite> m_sprites;
        std::vector<bool> m_enabled;
        std::unordered_set<uint32_t> m_sprites_need_update;

        std::unordered_map<uint32_t, SpriteAnimSequence> m_active_anim_sequences;
    };
}
