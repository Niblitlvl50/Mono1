
#include "TextureFactory.h"
#include "Texture.h"
#include "IImage.h"
#include "ImageFactory.h"

#include <cstdio>
#include <unordered_map>
#include <string>

namespace
{
    // This is where all the weak pointers goes, that points to the allocated textures!
    std::unordered_map<std::string, std::weak_ptr<mono::ITexture>> g_texture_storage;

    mono::ITexturePtr GetTextureFromCache(const char* texture_name)
    {
        auto it = g_texture_storage.find(texture_name);
        if(it != g_texture_storage.end())
        {
            mono::ITexturePtr texture = it->second.lock();
            if(texture)
                return texture;
        }

        std::printf("TextureFactory - Unable to find '%s' in cache.\n", texture_name);
        return nullptr;
    }

    mono::ITexturePtr CreateAndCacheTexture(const System::IImagePtr& image, const char* texture_name)
    {
        const auto deleter = [texture_name](mono::ITexture* ptr) {
            g_texture_storage.erase(texture_name);
            delete ptr;
        };

        mono::ITexturePtr texture(new mono::Texture(image), deleter);
        g_texture_storage[texture_name] = texture;

        return texture;
    }
}

mono::ITexturePtr mono::CreateTexture(const char* source)
{
    mono::ITexturePtr texture = GetTextureFromCache(source);
    if(texture)
        return texture;
    
    const System::IImagePtr image = System::LoadImage(source);
    return CreateAndCacheTexture(image, source);
}

mono::ITexturePtr mono::CreateTextureFromData(const unsigned char* data, int data_length, const char* cache_name)
{
    if(cache_name)
    {
        mono::ITexturePtr texture = GetTextureFromCache(cache_name);
        if(texture)
            return texture;

        const System::IImagePtr image = System::LoadImageFromData(data, data_length);
        return CreateAndCacheTexture(image, cache_name);
    }
    else
    {
        const System::IImagePtr image = System::LoadImageFromData(data, data_length);
        return std::make_shared<Texture>(image);    
    }
}

mono::ITexturePtr mono::CreateTexture(const unsigned char* data, int width, int height, int color_components)
{
    const System::IImagePtr image = System::CreateImage(data, width, height, color_components);
    return std::make_shared<Texture>(image);
}
