
#pragma once

#include "IGameSystem.h"
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
        bool flip_vertical = false;
        bool flip_horizontal = false;
        int animation_id = 0;
        int layer = 0;
    };

    using ForEachSpriteFunc = std::function<void (mono::ISprite* sprite, int layer, uint32_t sprite_id)>;

    class SpriteSystem : public mono::IGameSystem
    {
    public:

        SpriteSystem(size_t n_sprites, mono::TransformSystem* transform_system);
        ~SpriteSystem();

        mono::Sprite* AllocateSprite(uint32_t sprite_id);
        mono::ISprite* AllocateSprite(uint32_t sprite_id, const SpriteComponents& sprite_args);
        bool IsAllocated(uint32_t sprite_id);
        void SetSpriteData(uint32_t sprite_id, const SpriteComponents& component_data);
        void ReleaseSprite(uint32_t sprite_id);

        mono::Sprite* GetSprite(uint32_t sprite_id);
        int GetSpriteLayer(uint32_t sprite_id) const;
        void ForEachSprite(ForEachSpriteFunc func);

        uint32_t Id() const override;
        const char* Name() const override;
        uint32_t Capacity() const override;
        void Update(const UpdateContext& update_context) override;

    private:

        mono::TransformSystem* m_transform_system;
        std::vector<mono::Sprite> m_sprites;
        std::vector<int> m_sprite_layers;
        std::vector<bool> m_alive;
    };
}
