
#pragma once

#include "IGameSystem.h"
#include "Rendering/Color.h"
#include <vector>

namespace mono
{
    class ISprite;
    class Sprite;
    class TransformSystem;

    struct SpriteComponents
    {
        const char* sprite_file = nullptr;
        mono::Color::RGBA shade;
        bool flip_vertical = false;
        bool flip_horizontal = false;
        int animation_id = 0;
    };

    using ForEachSpriteFunc = void(*)(mono::ISprite* sprite, uint32_t sprite_id, void* context);

    class SpriteSystem : public mono::IGameSystem
    {
    public:

        SpriteSystem(size_t n_sprites, mono::TransformSystem* transform_system);
        ~SpriteSystem();

        mono::Sprite* AllocateSprite(uint32_t sprite_id);
        mono::ISprite* AllocateSprite(uint32_t sprite_id, const SpriteComponents& sprite_args);
        void SetSpriteData(uint32_t sprite_id, const SpriteComponents& component_data);
        void ReleaseSprite(uint32_t sprite_id);

        mono::Sprite* GetSprite(uint32_t sprite_id);
        void RunForEachSprite(ForEachSpriteFunc func, void* context);

        uint32_t Id() const override;
        const char* Name() const override;
        uint32_t Capacity() const override;
        void Update(uint32_t delta_ms) override;

    private:

        mono::TransformSystem* m_transform_system;
        std::vector<mono::Sprite> m_sprites;
        std::vector<bool> m_alive;
    };
}
