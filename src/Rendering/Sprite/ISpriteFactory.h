
#pragma once

#include <memory>

namespace mono
{
    using ISpritePtr = std::unique_ptr<class ISprite>;

    class ISpriteFactory
    {
    public:

        virtual ~ISpriteFactory() = default;
        
        virtual mono::ISpritePtr CreateSprite(const char* sprite_file) const = 0;
        virtual mono::ISpritePtr CreateSpriteFromRaw(const char* sprite_raw) const = 0;
        virtual bool CreateSprite(class Sprite& sprite, const char* sprite_file) const = 0;
        virtual const struct SpriteData* GetSpriteDataForFile(const char* sprite_file) const = 0;
    };
}
