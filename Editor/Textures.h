
#pragma once

#include <string.h>

namespace editor
{
    static const char* avalible_textures[] = {
        "textures/placeholder.png",
        "textures/stone_tileable.png",
        "textures/gray_stone.png",
        "textures/dark_stone.png",
        "textures/brown_stone.png",
        "textures/lava1.png",
        "textures/lava2.png"
        };

    constexpr int n_textures = 7;

    inline int FindTextureIndex(const char* texture)
    {
        for(int index = 0; index < n_textures; ++index)
        {
            const bool found = std::strstr(texture, avalible_textures[index]) != nullptr;
            if(found)
                return index;
        }

        return -1;
    }
}
