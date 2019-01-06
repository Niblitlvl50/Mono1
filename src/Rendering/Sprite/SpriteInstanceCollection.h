
#pragma once

#include <vector>

namespace mono
{
    class ISprite;
    class Sprite;

    using ForEachSpriteFunc = void(*)(mono::ISprite* sprite);

    class SpriteInstanceCollection
    {
    public:

        SpriteInstanceCollection(size_t n_sprites);
        ~SpriteInstanceCollection();

        mono::Sprite* AllocateSprite();
        void ReleaseSprite(mono::Sprite* sprite);

        void RunForEachSprite(ForEachSpriteFunc func);
        void Update(uint32_t delta);

    private:

        std::vector<mono::Sprite> m_sprites;
        std::vector<size_t> m_free_indices;
    };
}
