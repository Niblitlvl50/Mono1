
#pragma once

#include "Rendering/RenderPtrFwd.h"

namespace mono
{
    class Sprite;

    ISpritePtr CreateSprite(const char* sprite_file);
    bool CreateSprite(Sprite& sprite, const char* sprite_file);
}
