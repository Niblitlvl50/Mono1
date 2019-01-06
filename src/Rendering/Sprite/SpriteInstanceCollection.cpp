
#include "SpriteInstanceCollection.h"
#include "Sprite.h"

#include <algorithm>
#include <numeric>

using namespace mono;

SpriteInstanceCollection::SpriteInstanceCollection(size_t n_sprites)
{
    m_sprites.resize(n_sprites);
    m_free_indices.resize(n_sprites);

    std::iota(m_free_indices.begin(), m_free_indices.end(), 0);
    std::reverse(m_free_indices.begin(), m_free_indices.end());
}

SpriteInstanceCollection::~SpriteInstanceCollection()
{ }

mono::Sprite* SpriteInstanceCollection::AllocateSprite()
{
    assert(!m_free_indices.empty());

    const size_t free_index = m_free_indices.back();
    m_free_indices.pop_back();

    return &m_sprites[free_index];
}

void SpriteInstanceCollection::ReleaseSprite(mono::Sprite* sprite)
{
    const auto find_func = [sprite](const mono::Sprite& other_sprite) {
        return &other_sprite == sprite;
    };

    const auto found_it = std::find_if(m_sprites.begin(), m_sprites.end(), find_func);
    if(found_it != m_sprites.end())
    {
        const size_t free_index = std::distance(m_sprites.begin(), found_it);
        m_free_indices.push_back(free_index);
    }
}

void SpriteInstanceCollection::Update(uint32_t delta)
{
    for(Sprite& sprite : m_sprites)
        sprite.doUpdate(delta);
}

void SpriteInstanceCollection::RunForEachSprite(ForEachSpriteFunc func)
{
    for(mono::Sprite& sprite : m_sprites)
        func(&sprite);
}
