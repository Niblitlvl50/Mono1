
#pragma once

#include "IGameSystem.h"
#include "Math/Vector.h"
#include "Rendering/Color.h"

#include <vector>
#include <functional>

namespace mono
{
    class ISprite;
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

    using ForEachSpriteFunc = std::function<void (mono::ISprite* sprite, int layer, uint32_t sprite_id)>;

    class SpriteSystem : public mono::IGameSystem
    {
    public:

        SpriteSystem(uint32_t n_sprites, mono::TransformSystem* transform_system);

        mono::Sprite* AllocateSprite(uint32_t sprite_id);
        mono::ISprite* AllocateSprite(uint32_t sprite_id, const SpriteComponents& sprite_args);
        bool IsAllocated(uint32_t sprite_id);
        void SetSpriteData(uint32_t sprite_id, const SpriteComponents& component_data);
        void ReleaseSprite(uint32_t sprite_id);

        mono::Sprite* GetSprite(uint32_t sprite_id);
        int GetSpriteLayer(uint32_t sprite_id) const;
        void SetSpriteLayer(uint32_t sprite_id, int new_layer);
        float GetSpriteSortOffset(uint32_t sprite_id) const;
        void SetSpriteEnabled(uint32_t sprite_id, bool enabled);
        void ForEachSprite(const ForEachSpriteFunc& func);

        uint32_t Id() const override;
        const char* Name() const override;
        void Update(const UpdateContext& update_context) override;

    private:

        mono::TransformSystem* m_transform_system;
        std::vector<mono::Sprite> m_sprites;
        std::vector<int> m_sprite_layers;
        std::vector<float> m_sprite_sort_offsets;
        std::vector<bool> m_enabled;
        std::vector<bool> m_alive;

        std::vector<uint32_t> m_sprites_need_update;
    };
}
