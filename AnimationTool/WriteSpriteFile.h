
#pragma once

#include "MonoFwd.h"
#include <vector>

namespace animator
{
    void WriteSpriteFile(const char* sprite_file, const std::vector<mono::AnimationSequence>& animations);
}
