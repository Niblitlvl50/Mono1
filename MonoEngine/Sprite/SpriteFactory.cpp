
//
// This is an example of a sprite file that you pass to this sprite factory

//
// An image with 2 rows and 5 columns and their indices.
//  _ _ _ _ _
// |0|1|2|3|4|
//  - - - - -
// |5|6|7|8|9|
//  - - - - -
//

#include "SpriteFactory.h"
#include "Sprite.h"

#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/TextureFactory.h"

#include "Math/Quad.h"
#include "System/SysFile.h"

#include "nlohmann_json/json.hpp"

namespace
{
    struct TextureData
    {
        int rows;
        int columns;
        int x;
        int y;
        int u;
        int v;
        int textureSizeX;
        int textureSizeY;
    };

    void GenerateTextureCoordinates(const TextureData& data, std::vector<math::Quad>& coordinates)
    {
        coordinates.reserve(data.rows * data.columns);

        // +1 because the textures coordinates is zero indexed
        const float x1 = float(data.x +1) / float(data.textureSizeX);
        const float y1 = float(data.y) / float(data.textureSizeY);
        const float x2 = float(data.u +1) / float(data.textureSizeX);
        const float y2 = float(data.v) / float(data.textureSizeY);
        const float sizex = x2 - x1;
        const float sizey = y2 - y1;

        const float xstep = sizex / data.columns;
        const float ystep = sizey / data.rows;

        for(int yindex = 0; yindex < data.rows; ++yindex)
        {
            for(int xindex = 0; xindex < data.columns; ++xindex)
            {
                const float x = xstep * float(xindex) + x1;
                const float y = ystep * float(yindex) + y1;

                coordinates.emplace_back(x, ystep + y, x + xstep, y);
            }
        }
    }

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

        TextureData data;
        data.rows    = json["rows"];
        data.columns = json["columns"];
        data.x       = json["x"];
        data.y       = json["y"];
        data.u       = json["u"];
        data.v       = json["v"];

        data.textureSizeX = texture->Width();
        data.textureSizeY = texture->Height();

        std::vector<math::Quad> textureCoordinates;
        GenerateTextureCoordinates(data, textureCoordinates);

        sprite.Init(texture, textureCoordinates);

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
