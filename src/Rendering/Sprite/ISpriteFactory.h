
#pragma once

#include "Rendering/RenderPtrFwd.h"

namespace mono
{
    class Sprite;

    class ISpriteFactory
    {
    public:

        virtual ~ISpriteFactory() = default;
        virtual mono::ISpritePtr CreateSprite(const char* sprite_file) const = 0;
        virtual mono::ISpritePtr CreateSpriteFromRaw(const char* sprite_raw) const = 0;
        virtual bool CreateSprite(Sprite& sprite, const char* sprite_file) const = 0;
    };

    const ISpriteFactory* GetSpriteFactory();
}
