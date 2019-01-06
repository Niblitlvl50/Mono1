
#pragma once

#include "ISpriteFactory.h"
#include <unordered_map>

namespace mono
{
    class SpriteFactoryImpl : public mono::ISpriteFactory
    {
    public:

        SpriteFactoryImpl(class SpriteInstanceCollection* sprite_collection, float pixels_per_meter);

        mono::ISpritePtr CreateSprite(const char* sprite_file) const override;
        mono::ISpritePtr CreateSpriteFromRaw(const char* sprite_raw) const override;
        bool CreateSprite(class Sprite& sprite, const char* sprite_file) const override;
    
        SpriteInstanceCollection* m_sprite_collection;
        const float m_pixels_per_meter;
        mutable std::unordered_map<uint32_t, struct SpriteData> m_sprite_data_cache;
    };
}
