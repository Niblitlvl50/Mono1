
#pragma once

#include "ISprite.h"
#include "ISpriteFactory.h"
#include "Math/Vector.h"
#include <unordered_map>
#include <string>
#include <vector>

namespace mono
{
    struct SpriteAnimation
    {
        std::string name;
        bool loop;
        std::vector<int> frames;
    };

    struct SpriteData
    {
        std::string texture_file;
        math::Vector texture_size;
        std::vector<SpriteFrame> sprite_frames;
        std::vector<SpriteAnimation> animations;
    };

    class SpriteFactoryImpl : public mono::ISpriteFactory
    {
    public:

        SpriteFactoryImpl(float pixels_per_meter);

        mono::ISpritePtr CreateSprite(const char* sprite_file) const override;
        mono::ISpritePtr CreateSpriteFromRaw(const char* sprite_raw) const override;
        bool CreateSprite(class Sprite& sprite, const char* sprite_file) const override;
    
        const float m_pixels_per_meter;
        mutable std::unordered_map<uint32_t, SpriteData> m_sprite_data_cache;
    };
}
