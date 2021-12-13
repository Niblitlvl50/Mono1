
#include "TextureFactoryImpl.h"
#include "Texture.h"
#include "System/Hash.h"
#include "System/System.h"
#include "System/File.h"

#include <cstdio>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

using namespace mono;

mono::ITexturePtr TextureFactoryImpl::CreateTexture(const char* texture_name) const
{
    if(strlen(texture_name) == 0)
        return nullptr;

    const uint32_t texture_hash = hash::Hash(texture_name);
    mono::ITexturePtr texture = GetTextureFromCache(texture_hash);
    if(texture)
        return texture;

    System::Log("TextureFactory|Creating texture '%s'.", texture_name);
    return CreateAndCacheTexture(texture_name, texture_hash);
}

mono::ITexturePtr TextureFactoryImpl::CreateTextureFromData(const byte* data, int data_length, const char* cache_name) const
{
    if(cache_name)
    {
        const uint32_t texture_hash = hash::Hash(cache_name);
        mono::ITexturePtr texture = GetTextureFromCache(texture_hash);
        if(texture)
            return texture;

        return CreateAndCacheTexture(data, data_length, texture_hash);
    }
    else
    {
        int width;
        int height;
        int components;
        stbi_uc* loaded_image_data = stbi_load_from_memory(data, data_length, &width, &height, &components, 0);
        if(!loaded_image_data)
        {
            System::Log("TextureFactory|Unable to load from data chunk.");
            throw std::runtime_error("Unable to load image!");
        }

        ITexturePtr texture = std::make_shared<mono::TextureImpl>(width, height, components, loaded_image_data);
        stbi_image_free(loaded_image_data);

        return texture;
    }
}

mono::ITexturePtr TextureFactoryImpl::CreateTexture(const byte* data, int width, int height, int color_components) const
{
    return std::make_shared<TextureImpl>(width, height, color_components, data);
}

mono::ITexturePtr TextureFactoryImpl::CreateFromNativeHandle(uint32_t native_handle) const
{
    sg_image handle;
    handle.id = native_handle;
    return std::make_shared<TextureImpl>(handle);
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
    const std::vector<byte>& source_file_data = file::FileReadAll(source_file);
    return CreateAndCacheTexture(source_file_data.data(), source_file_data.size(), texture_hash);
}

mono::ITexturePtr TextureFactoryImpl::CreateAndCacheTexture(const byte* data, int data_length, uint32_t texture_hash) const
{
    int width;
    int height;
    int components;
    stbi_uc* loaded_image_data = stbi_load_from_memory(data, data_length, &width, &height, &components, 0);
    if(!loaded_image_data)
    {
        System::Log("TextureFactory|Unable to load from data chunk");
        throw std::runtime_error("Unable to load image!");
    }

    const auto deleter = [this, texture_hash](mono::ITexture* ptr) {
        m_texture_storage.erase(texture_hash);
        delete ptr;
    };

    mono::ITexturePtr texture(new mono::TextureImpl(width, height, components, loaded_image_data), deleter);
    m_texture_storage[texture_hash] = texture;

    stbi_image_free(loaded_image_data);

    return texture;
}
