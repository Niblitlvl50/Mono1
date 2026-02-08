
//
// This is an example of a sprite file that you pass to this sprite factory
// ...

//
// y
// ^
// |
// |
// 0-------> x
//

#include "SpriteFactory.h"
#include "Sprite.h"

#include "SpriteData.h"
#include "Serialize.h"

#include "Rendering/RenderSystem.h"
#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/ITextureFactory.h"

#include "Math/Quad.h"
#include "Math/Serialize.h"
#include "System/File.h"
#include "System/System.h"
#include "System/Hash.h"

#include <unordered_map>
#include <string>

#include "nlohmann/json.hpp"

namespace
{
    mono::SpriteData LoadSpriteData(const char* sprite_raw_data, float pixels_per_meter, uint32_t sprite_hash)
    {
        const nlohmann::json& json = nlohmann::json::parse(sprite_raw_data);

        mono::SpriteData serialize_data = json.get<mono::SpriteData>();
        serialize_data.hash = sprite_hash;

        for(auto& frame : serialize_data.frames)
            frame.size = (frame.size / pixels_per_meter);

        return serialize_data;
    }
}

using namespace mono;

SpriteFactoryImpl::SpriteFactoryImpl(float pixels_per_meter)
    : m_pixels_per_meter(pixels_per_meter)
{ }

mono::ISpritePtr SpriteFactoryImpl::CreateSprite(const char* sprite_file) const
{
    auto sprite = std::make_unique<mono::Sprite>();
    const bool result = CreateSprite(*sprite.get(), sprite_file);
    if(result)
        return sprite;

    return nullptr;
}

mono::ISpritePtr SpriteFactoryImpl::CreateSpriteFromRaw(const char* sprite_raw) const
{
    const uint32_t sprite_raw_hash = hash::Hash(sprite_raw);

    auto it = m_sprite_data_cache.find(sprite_raw_hash);
    if(it == m_sprite_data_cache.end())
    {
        mono::SpriteData sprite_data = LoadSpriteData(sprite_raw, m_pixels_per_meter, sprite_raw_hash);
        it = m_sprite_data_cache.insert({sprite_raw_hash, sprite_data}).first;
    }

    const mono::SpriteData& sprite_data = it->second;
    mono::ITexturePtr texture = mono::RenderSystem::GetTextureFactory()->CreateTexture(sprite_data.texture_file.c_str());
    return std::make_unique<mono::Sprite>(&sprite_data, texture);
}

bool SpriteFactoryImpl::CreateSprite(mono::Sprite& sprite, const char* sprite_file) const
{
    const mono::SpriteData* sprite_data = GetSpriteDataForFile(sprite_file);
    if(!sprite_data)
        return false;

    mono::ITexturePtr texture = mono::RenderSystem::GetTextureFactory()->CreateTexture(sprite_data->texture_file.c_str());
    sprite.Init(sprite_data, texture);
    return true;
}

const mono::SpriteData* SpriteFactoryImpl::GetSpriteDataForFile(const char* sprite_file) const
{
    const uint32_t sprite_filename_hash = hash::Hash(sprite_file);

    auto it = m_sprite_data_cache.find(sprite_filename_hash);
    if(it == m_sprite_data_cache.end())
    {
        const bool file_exists = file::Exists(sprite_file);
        if(!file_exists)
        {
            System::Log("spritefactory|Sprite file does not exist. [%s]", sprite_file);
            return nullptr;
        }

        std::vector<byte> file_data = file::FileReadAll(sprite_file);
        file_data.push_back('\0');

        mono::SpriteData sprite_data = LoadSpriteData((const char*)file_data.data(), m_pixels_per_meter, sprite_filename_hash);
        it = m_sprite_data_cache.insert({sprite_filename_hash, sprite_data}).first;
    }

    return &it->second;
}
