
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

#include "nlohmann_json/json.hpp"

namespace
{
    bool LoadSpriteData(mono::Sprite& sprite, const char* sprite_file)
    {
        File::FilePtr file = File::OpenAsciiFile(sprite_file);
        if(!file)
            return false;

        std::vector<byte> file_data;
        File::FileRead(file, file_data);

        const nlohmann::json& json = nlohmann::json::parse(file_data);
        
        const std::string& texture_file = json["texture"];
        mono::ITexturePtr texture = mono::CreateTexture(texture_file.c_str());
        const float texture_width = texture->Width();
        const float texture_height = texture->Height();

        const nlohmann::json& frames = json["frames"];
        std::vector<math::Quad> texture_coordinates;
        texture_coordinates.reserve(frames.size());

        for(const nlohmann::json& frame : frames)
        {
            const float x = float(frame["x"]) / texture_width;
            const float y = float(frame["y"]) / texture_height;
            const float w = float(frame["w"]) / texture_width;
            const float h = float(frame["h"]) / texture_height;

            texture_coordinates.emplace_back(x, y + h, x + w, y);
        }

        sprite.Init(texture, texture_coordinates);

        for(const auto& animation : json["animations"])
        {
            const std::string& name = animation["name"];
            const bool loop = animation["loop"];
            const std::vector<int>& frames = animation["frames"];
            sprite.DefineAnimation(name.c_str(), frames, loop);
        }

        return true;
    }
}

mono::ISpritePtr mono::CreateSprite(const char* sprite_file)
{
    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>();
    const bool result = LoadSpriteData(*sprite.get(), sprite_file);
    if(result)
        return sprite;

    return nullptr;
}

bool mono::CreateSprite(mono::Sprite& sprite, const char* sprite_file)
{
    return LoadSpriteData(sprite, sprite_file);
}
