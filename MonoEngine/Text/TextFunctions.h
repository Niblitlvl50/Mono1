//
//  TextFunctions.h
//  Mono1
//
//  Created by Niblit on 2012-07-02.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "TextFwd.h"
#include "SysTypes.h"

namespace Math
{
    struct Vector2f;
}

namespace mono
{    
    struct TextDefinition
    {
        FloatVector vertices;
        FloatVector texcoords;
        int chars;
    };
    
    struct CharData
    {
        float xadvance;
        float xoffset;
        float yoffset;
        float width;
        float height;
        
        float texCoordX0;
        float texCoordY0;
        float texCoordX1;
        float texCoordY1;
    };    
    
    void LoadFont(const std::string& font, float size);

    FloatVector GenerateVerticesFromString(const std::string& text, const Math::Vector2f& pos, bool center);
    FloatVector GenerateTextureCoordinates(const std::string& text);
    
    void DrawTextFromDefinitions(const TextDefCollection& collection);
}

