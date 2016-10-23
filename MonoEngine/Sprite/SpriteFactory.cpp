//
//  SpriteFactory.cpp
//  MonoiOS
//
//  Created by Niklas Damberg on 21/07/16.
//
//


//! This is an example of a sprite file that you pass to this sprite factory

//!
//! texture = "sheet.png"
//! rows = 3
//! columns = 4
//!
//! x = 1
//! y = 1
//! u = 128
//! v = 98
//!
//! animations = { }
//! animations[0] = { 0, -1 }
//! animations[1] = { 1, 100, 2, 100, 3, 100 }
//! animations[2] = { 4, 100, 5, 100, 6, 100, 7, 100 }
//! animations[3] = { 8, 100, 9, 100, 10, 100, 11, 100 }
//!

//!
//! An image with 2 rows and 5 columns and their indices.
//!  _ _ _ _ _
//! |0|1|2|3|4|
//!  - - - - -
//! |5|6|7|8|9|
//!  - - - - -
//!


#include "SpriteFactory.h"

#include "Texture/ITexture.h"
#include "Texture/TextureFactory.h"

#include "Math/Quad.h"
#include "Sprite.h"

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
}


mono::ISpritePtr mono::CreateSprite(const char* sprite_file)
{
    File::FilePtr file = File::OpenAsciiFile(sprite_file);
    if(!file)
        return nullptr;

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

    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(texture, textureCoordinates);

    int id = 0;

    for(const auto& animation : json["animations"])
    {
        const bool loop = animation["loop"];
        const std::vector<int>& frames = animation["frames"];
        sprite->DefineAnimation(id, frames, loop);

        id++;
    }

    return sprite;
}
