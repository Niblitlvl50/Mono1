
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
        const nlohmann::json& texture_size = json["texture_size"];

        mono::SpriteData sprite_data;
        sprite_data.hash = sprite_hash;
        sprite_data.texture_file = json["texture"];
        sprite_data.source_folder = json.value("source_folder", "");
        sprite_data.texture_size = math::Vector(texture_size["w"], texture_size["h"]);

        const nlohmann::json& frames = json["frames"];
        const nlohmann::json& frames_offsets = json["frames_offsets"];

        sprite_data.frames.reserve(frames.size());

        for(size_t index = 0; index < frames.size(); ++index)
        {
            const nlohmann::json& frame = frames[index];

            const float x = float(frame["x"]) / sprite_data.texture_size.x;
            const float y = float(frame["y"]) / sprite_data.texture_size.y;
            const float w = float(frame["w"]) / sprite_data.texture_size.x;
            const float h = float(frame["h"]) / sprite_data.texture_size.y;

            const nlohmann::json& frame_offset = frames_offsets[index];

            mono::SpriteFrame sprite_frame;
            sprite_frame.center_offset.x = frame_offset["x"];
            sprite_frame.center_offset.y = frame_offset["y"];
            sprite_frame.uv_upper_left = math::Vector(x, y + h);
            sprite_frame.uv_lower_right = math::Vector(x + w, y);

            const float width =
                (sprite_frame.uv_lower_right.x - sprite_frame.uv_upper_left.x) * sprite_data.texture_size.x;
            const float height =
                (sprite_frame.uv_upper_left.y - sprite_frame.uv_lower_right.y) * sprite_data.texture_size.y;

            sprite_frame.size = math::Vector(width, height) / pixels_per_meter;
            sprite_data.frames.push_back(sprite_frame);
        }

        const nlohmann::json& animations = json["animations"];
        sprite_data.animations.reserve(animations.size());

        for(const auto& animation : animations)
        {
            mono::SpriteAnimation sprite_animation;
            sprite_animation.name = animation["name"];
            sprite_animation.looping = animation["loop"];
            sprite_animation.frame_duration = animation["frame_duration"];
            sprite_animation.frames = animation["frames"].get<std::vector<int>>();

            sprite_data.animations.push_back(sprite_animation);
        }

        return sprite_data;
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
    mono::ITexturePtr texture = mono::GetTextureFactory()->CreateTexture(sprite_data.texture_file.c_str());
    return std::make_unique<mono::Sprite>(&sprite_data, texture);
}

bool SpriteFactoryImpl::CreateSprite(mono::Sprite& sprite, const char* sprite_file) const
{
    const mono::SpriteData* sprite_data = GetSpriteDataForFile(sprite_file);
    if(!sprite_data)
        return false;

    mono::ITexturePtr texture = mono::GetTextureFactory()->CreateTexture(sprite_data->texture_file.c_str());
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

        file::FilePtr file = file::OpenAsciiFile(sprite_file);
        if(!file)
            return nullptr;

        std::vector<byte> file_data = file::FileRead(file);
        file_data.push_back('\0');

        mono::SpriteData sprite_data = LoadSpriteData((const char*)file_data.data(), m_pixels_per_meter, sprite_filename_hash);
        it = m_sprite_data_cache.insert({sprite_filename_hash, sprite_data}).first;
    }

    return &it->second;
}
