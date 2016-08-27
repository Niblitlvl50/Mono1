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

#include "LuaState.h"
#include "LuaFunctions.h"

#include "ITexture.h"
#include "TextureFactory.h"

#include "Quad.h"
#include "Sprite.h"

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
    lua::LuaState config(sprite_file);

    const std::string& texture_file = lua::GetValue<std::string>(config, "texture");
    mono::ITexturePtr texture = mono::CreateTexture(texture_file.c_str());

    TextureData data;
    data.rows    = lua::GetValue<int>(config, "rows");
    data.columns = lua::GetValue<int>(config, "columns");
    data.x       = lua::GetValue<int>(config, "x");
    data.y       = lua::GetValue<int>(config, "y");
    data.u       = lua::GetValue<int>(config, "u");
    data.v       = lua::GetValue<int>(config, "v");

    data.textureSizeX = texture->Width();
    data.textureSizeY = texture->Height();

    std::vector<math::Quad> textureCoordinates;
    GenerateTextureCoordinates(data, textureCoordinates);

    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(texture, textureCoordinates);

    const lua::MapIntIntTable& animations = lua::GetTableMap<int, int>(config, "animations");
    const lua::MapIntStringTable& attributes = lua::GetTableMap<int, std::string>(config, "attributes");

    for(const auto& animation : animations)
    {
        const int key = animation.first;
        const lua::IntTable& values = animation.second;

        auto attribute = attributes.find(key);
        if(attribute == attributes.end())
            sprite->DefineAnimation(key, values, true);
        else
            sprite->DefineAnimation(key, values, attribute->second);
    }

    return sprite;
}
