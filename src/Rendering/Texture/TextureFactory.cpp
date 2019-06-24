
#include "TextureFactory.h"
#include "Texture.h"
#include "IImage.h"
#include "ImageFactory.h"
#include "Hash.h"

#include <cstdio>
#include <unordered_map>

namespace
{
    // This is where all the weak pointers goes, that points to the allocated textures!
    std::unordered_map<unsigned int, std::weak_ptr<mono::ITexture>> g_texture_storage;

    mono::ITexturePtr GetTextureFromCache(unsigned int texture_hash)
    {
        auto it = g_texture_storage.find(texture_hash);
        if(it != g_texture_storage.end())
        {
            mono::ITexturePtr texture = it->second.lock();
            if(texture)
                return texture;
        }

        return nullptr;
    }

    mono::ITexturePtr CreateAndCacheTexture(const mono::IImagePtr& image, unsigned int texture_hash)
    {
        const auto deleter = [texture_hash](mono::ITexture* ptr) {
            g_texture_storage.erase(texture_hash);
            delete ptr;
        };

        mono::ITexturePtr texture(new mono::Texture(image), deleter);
        g_texture_storage[texture_hash] = texture;

        return texture;
    }
}

mono::ITexturePtr mono::CreateTexture(const char* texture_name)
{
    const unsigned int texture_hash = mono::Hash(texture_name);

    mono::ITexturePtr texture = GetTextureFromCache(texture_hash);
    if(texture)
        return texture;
    else
        std::printf("TextureFactory|Unable to find '%s' in cache\n", texture_name);

    const mono::IImagePtr image = mono::LoadImage(texture_name);
    return CreateAndCacheTexture(image, texture_hash);
}

mono::ITexturePtr mono::CreateTextureFromData(const byte* data, int data_length, const char* cache_name)
{
    if(cache_name)
    {
        const unsigned int texture_hash = mono::Hash(cache_name);

        mono::ITexturePtr texture = GetTextureFromCache(texture_hash);
        if(texture)
            return texture;

        const mono::IImagePtr image = mono::LoadImageFromData(data, data_length);
        return CreateAndCacheTexture(image, texture_hash);
    }
    else
    {
        const mono::IImagePtr image = mono::LoadImageFromData(data, data_length);
        return std::make_shared<Texture>(image);    
    }
}

mono::ITexturePtr mono::CreateTexture(const byte* data, int width, int height, int color_components)
{
    const mono::IImagePtr image = mono::CreateImage(data, width, height, color_components);
    return std::make_shared<Texture>(image);
}
