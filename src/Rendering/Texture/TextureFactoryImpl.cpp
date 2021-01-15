
#include "TextureFactoryImpl.h"
#include "Texture.h"
#include "Util/Hash.h"
#include "System/System.h"

#include <cstdio>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image.h"

using namespace mono;

namespace
{
    mono::ITexturePtr CreateTextureFromData(const unsigned char* data, int data_length)
    {
        int width;
        int height;
        int components;
        std::unique_ptr<unsigned char> image_data(stbi_load_from_memory(data, data_length, &width, &height, &components, 0));
        if(!image_data)
        {
            System::Log("TextureFactory|Unable to load from data chunk\n");
            throw std::runtime_error("Unable to load image!");
        }

        return std::make_shared<mono::Texture>(width, height, components, image_data.get());
    }
}

mono::ITexturePtr TextureFactoryImpl::CreateTexture(const char* texture_name) const
{
    const uint32_t texture_hash = mono::Hash(texture_name);

    mono::ITexturePtr texture = GetTextureFromCache(texture_hash);
    if(texture)
        return texture;

    System::Log("TextureFactory|Creating texture '%s'.\n", texture_name);
    return CreateAndCacheTexture(texture_name, texture_hash);
}

mono::ITexturePtr TextureFactoryImpl::CreateTextureFromData(const byte* data, int data_length, const char* cache_name) const
{
    if(cache_name)
    {
        const uint32_t texture_hash = mono::Hash(cache_name);

        mono::ITexturePtr texture = GetTextureFromCache(texture_hash);
        if(texture)
            return texture;

        return CreateAndCacheTexture(data, data_length, texture_hash);
    }
    else
    {
        return ::CreateTextureFromData(data, data_length);
    }}

mono::ITexturePtr TextureFactoryImpl::CreateTexture(const byte* data, int width, int height, int color_components) const
{
    return std::make_shared<Texture>(width, height, color_components, data);
}

mono::ITexturePtr TextureFactoryImpl::GetTextureFromCache(uint32_t texture_hash) const
{
    auto it = m_texture_storage.find(texture_hash);
    if(it != m_texture_storage.end())
    {
        mono::ITexturePtr texture = it->second.lock();
        if(texture)
            return texture;
    }

    return nullptr;
}

mono::ITexturePtr TextureFactoryImpl::CreateAndCacheTexture(const char* source_file, uint32_t texture_hash) const
{
    int width;
    int height;
    int components;
    std::unique_ptr<unsigned char> image_data(stbi_load(source_file, &width, &height, &components, 0));
    if(!image_data)
    {
        System::Log("TextureFactory|Unable to load '%s'\n", source_file);
        throw std::runtime_error("Unable to load image!");
    }

    const auto deleter = [this, texture_hash](mono::ITexture* ptr) {
        m_texture_storage.erase(texture_hash);
        delete ptr;
    };

    mono::ITexturePtr texture(new mono::Texture(width, height, components, image_data.get()), deleter);
    m_texture_storage[texture_hash] = texture;

    return texture;
}

mono::ITexturePtr TextureFactoryImpl::CreateAndCacheTexture(const unsigned char* data, int data_length, uint32_t texture_hash) const
{
    int width;
    int height;
    int components;
    std::unique_ptr<unsigned char> image_data(stbi_load_from_memory(data, data_length, &width, &height, &components, 0));
    if(!image_data)
    {
        System::Log("TextureFactory|Unable to load from data chunk\n");
        throw std::runtime_error("Unable to load image!");
    }

    const auto deleter = [this, texture_hash](mono::ITexture* ptr) {
        m_texture_storage.erase(texture_hash);
        delete ptr;
    };

    mono::ITexturePtr texture(new mono::Texture(width, height, components, image_data.get()), deleter);
    m_texture_storage[texture_hash] = texture;

    return texture;
}

