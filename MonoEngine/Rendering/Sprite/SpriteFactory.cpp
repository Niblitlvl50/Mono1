
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

#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/TextureFactory.h"

#include "Math/Quad.h"
#include "System/File.h"
#include "Hash.h"

#include <unordered_map>
#include <string>

#include "nlohmann/json.hpp"

namespace
{
    struct SpriteAnimation
    {
        std::string name;
        bool loop;
        std::vector<int> frames;
    };

    struct SpriteData
    {
        std::string texture_file;
        math::Vector texture_size;
        std::vector<mono::SpriteFrame> sprite_frames;
        std::vector<SpriteAnimation> animations;
    };

    std::unordered_map<unsigned int, SpriteData> g_sprite_data_cache;

    SpriteData LoadSpriteData(const char* sprite_raw_data)
    {
        const nlohmann::json& json = nlohmann::json::parse(sprite_raw_data);
        
        const nlohmann::json& texture_size = json["texture_size"];

        SpriteData sprite_data;
        sprite_data.texture_file = json["texture"];
        sprite_data.texture_size = math::Vector(texture_size["w"], texture_size["h"]);

        const nlohmann::json& frames = json["frames"];
        sprite_data.sprite_frames.reserve(frames.size());

        for(const nlohmann::json& frame : frames)
        {
            const float x = float(frame["x"]) / sprite_data.texture_size.x;
            const float y = float(frame["y"]) / sprite_data.texture_size.y;
            const float w = float(frame["w"]) / sprite_data.texture_size.x;
            const float h = float(frame["h"]) / sprite_data.texture_size.y;

            mono::SpriteFrame sprite_frame;
            sprite_frame.center_offset = math::ZeroVec;
            sprite_frame.texture_coordinates = math::Quad(x, y + h, x + w, y);

            const float width =
                (sprite_frame.texture_coordinates.mB.x - sprite_frame.texture_coordinates.mA.x) * sprite_data.texture_size.x;
            const float height =
                (sprite_frame.texture_coordinates.mA.y - sprite_frame.texture_coordinates.mB.y) * sprite_data.texture_size.y;

            sprite_frame.size = math::Vector(width, height);
            sprite_data.sprite_frames.push_back(sprite_frame);
        }

        const nlohmann::json& animations = json["animations"];
        sprite_data.animations.reserve(animations.size());

        for(const auto& animation : animations)
        {
            SpriteAnimation sprite_animation;
            sprite_animation.name = animation["name"];
            sprite_animation.loop = animation["loop"];

            std::vector<int> frames = animation["frames"];
            sprite_animation.frames = std::move(frames);

            sprite_data.animations.push_back(sprite_animation);
        }

        return sprite_data;
    }
}

void mono::ClearSpriteCache()
{
    g_sprite_data_cache.clear();
}

mono::ISpritePtr mono::CreateSprite(const char* sprite_file)
{
    std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
    const bool result = CreateSprite(*sprite.get(), sprite_file);
    if(result)
        return sprite;

    return nullptr;
}

mono::ISpritePtr mono::CreateSpriteFromRaw(const char* sprite_raw)
{
    const SpriteData& sprite_data = LoadSpriteData(sprite_raw);    
    mono::ITexturePtr texture = mono::CreateTexture(sprite_data.texture_file.c_str());

    std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
    sprite->Init(texture, sprite_data.sprite_frames);

    for(const SpriteAnimation& animation : sprite_data.animations)
        sprite->DefineAnimation(animation.name.c_str(), animation.frames, animation.loop);

    return sprite;
}

bool mono::CreateSprite(mono::Sprite& sprite, const char* sprite_file)
{
    const unsigned int sprite_file_hash = mono::Hash(sprite_file);
    
    auto it = g_sprite_data_cache.find(sprite_file_hash);
    if(it == g_sprite_data_cache.end())
    {
        File::FilePtr file = File::OpenAsciiFile(sprite_file);
        if(!file)
            return false;

        std::vector<byte> file_data;
        File::FileRead(file, file_data);
        file_data.push_back('\0');

        const SpriteData& sprite_data = LoadSpriteData((const char*)file_data.data());
        it = g_sprite_data_cache.insert({sprite_file_hash, sprite_data}).first;
    }

    const SpriteData& sprite_data = it->second;

    mono::ITexturePtr texture = mono::CreateTexture(sprite_data.texture_file.c_str());
    sprite.Init(texture, sprite_data.sprite_frames);
    
    for(const SpriteAnimation& animation : sprite_data.animations)
        sprite.DefineAnimation(animation.name.c_str(), animation.frames, animation.loop);

    return true;
}
