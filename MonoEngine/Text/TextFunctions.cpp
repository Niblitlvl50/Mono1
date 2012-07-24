//
//  TextFunctions.cpp
//  Mono1
//
//  Created by Niblit on 2012-07-02.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#include "TextFunctions.h"
#include "SysOpenGL.h"
#include "SysFile.h"
#include "ITexture.h"
#include "Texture.h"
#include "TextureFactory.h"
#include "Vector2f.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

#include <map>
#include <iostream>
#include <cmath>


static mono::ITexturePtr fontTexture;

typedef std::map<char, mono::CharData> CharDataMap;
static CharDataMap charMap;

namespace
{
    Math::Vector2f MeasureString(const std::string& text)
    {
        float xsize = 0.0f;
        float ysize = 0.0f;
        
        for(std::string::const_iterator it = text.begin(), end = text.end(); it != end; ++it)
        {
            const char currentChar = *it;
            
            // Look up char in map.
            const CharDataMap::const_iterator foundChar = charMap.find(currentChar);
            if(foundChar == charMap.end())
                continue;
            
            const mono::CharData& data = foundChar->second;
            xsize += data.xadvance;
            ysize = std::max(std::abs(data.height), ysize);
        }
        
        return Math::Vector2f(xsize, ysize);
    }
}

void mono::LoadFont(const std::string& font, float size)
{
    File::FilePtr fontfile = File::OpenBinaryFile(font);
    const long filesize = File::FileSize(fontfile);
    
    ByteVector fontbuffer(filesize, 0);
    fread(&fontbuffer.front(), 1, filesize, fontfile.get());
    
    const int width = 512;
    const int height = 512;
    const int base = 32;
    const int chars = 224;

    byte bitmap[width * height];
    stbtt_bakedchar chardata[chars];
    
    const int result = stbtt_BakeFontBitmap(&fontbuffer.front(), 0, size, bitmap, width, height, base, chars, chardata);    
    fontTexture = mono::CreateTexture(bitmap, width, height, GL_ALPHA, GL_ALPHA);
    

    const float texCoordXMulti = 1.0f / width;
    const float texCoordYMulti = 1.0f / height;
    
    for(int index = 0; index < chars; ++index)
    {
        const stbtt_bakedchar& bakedchar = chardata[index];
        
        mono::CharData data;
        data.width = bakedchar.x1 - bakedchar.x0;
        data.height = bakedchar.y1 - bakedchar.y0;
        data.xadvance = bakedchar.xadvance;
        data.xoffset = bakedchar.xoff;
        data.yoffset = bakedchar.yoff + data.height;
        data.texCoordX0 = bakedchar.x0 * texCoordXMulti;
        data.texCoordY0 = bakedchar.y0 * texCoordYMulti;
        data.texCoordX1 = bakedchar.x1 * texCoordXMulti;
        data.texCoordY1 = bakedchar.y1 * texCoordYMulti;
        
        // For the correct char we need to add base to the index.
        const char thechar = index + base;
        charMap[thechar] = data;
    }
}

FloatVector mono::GenerateVerticesFromString(const std::string& text, const Math::Vector2f& pos, bool center)
{
    FloatVector vertices;
            
    float xposition = pos.mX;
    float yposition = pos.mY;
    
    if(center)
        xposition -= MeasureString(text).mX / 2.0f;
    
    for(std::string::const_iterator it = text.begin(), end = text.end(); it != end; ++it)
    {
        const char currentChar = *it;
        
        // Look up char in map.
        const CharDataMap::const_iterator foundChar = charMap.find(currentChar);
        if(foundChar == charMap.end())
            continue;
        
        const CharData& data = foundChar->second;
        
        const float x0 = xposition + data.xoffset;
        const float y0 = yposition - data.yoffset;
        const float x1 = x0 + data.width;
        const float y1 = y0 + data.height;

        
        // First triangle
        vertices.push_back(x0);
        vertices.push_back(y0);
        
        vertices.push_back(x1);
        vertices.push_back(y1);
        
        vertices.push_back(x0);
        vertices.push_back(y1);
        
        // Second triangle
        vertices.push_back(x0);
        vertices.push_back(y0);
        
        vertices.push_back(x1);
        vertices.push_back(y0);
        
        vertices.push_back(x1);
        vertices.push_back(y1);

        // Add x size of char so that the next char is placed a little bit to the right.
        xposition += data.xadvance;
    }
        
    return vertices;
}

FloatVector mono::GenerateTextureCoordinates(const std::string& text)
{
    FloatVector coordinates;
    
    for(std::string::const_iterator it = text.begin(), end = text.end(); it != end; ++it)
    {
        const char currentChar = *it;
        
        // Look up char in map.
        const CharDataMap::const_iterator foundChar = charMap.find(currentChar);
        if(foundChar == charMap.end())
            continue;
        
        const CharData& data = foundChar->second;

        // First triangle
        coordinates.push_back(data.texCoordX0);
        coordinates.push_back(data.texCoordY1);
        
        coordinates.push_back(data.texCoordX1);
        coordinates.push_back(data.texCoordY0);
        
        coordinates.push_back(data.texCoordX0);
        coordinates.push_back(data.texCoordY0);
        
        // Second triangle
        coordinates.push_back(data.texCoordX0);
        coordinates.push_back(data.texCoordY1);
        
        coordinates.push_back(data.texCoordX1);
        coordinates.push_back(data.texCoordY1);
        
        coordinates.push_back(data.texCoordX1);
        coordinates.push_back(data.texCoordY0);
    }
    
    return coordinates;
}

void mono::DrawTextFromDefinitions(const TextDefCollection& collection)
{
    if(collection.empty())
        return;
    
    fontTexture->Use();    
    glColor3f(1.0f, 1.0f, 1.0f);
        
    for(TextDefCollection::const_iterator it = collection.begin(), end = collection.end(); it != end; ++it)
    {
        const TextDefinition& def = *it;
                        
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        
        glVertexPointer(2, GL_FLOAT, 0, &def.vertices.front());
        glTexCoordPointer(2, GL_FLOAT, 0, &def.texcoords.front());
        
        // Number of chars in the text, times 3 since each triangle contains 3 vertices,
        // times 2 since each char containts two triangles.
        const int verticesToDraw = def.chars * 3 * 2;
        
        glDrawArrays(GL_TRIANGLES, 0, verticesToDraw);
                        
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    }
}


