
#pragma once

#include "Rendering/RenderPtrFwd.h"

namespace mono
{
    void ClearSpriteCache();

    ISpritePtr CreateSprite(const char* sprite_file);
    ISpritePtr CreateSpriteFromRaw(const char* sprite_raw);
    bool CreateSprite(class Sprite& sprite, const char* sprite_file);
}
