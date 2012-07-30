//
//  TextFunctions.h
//  Mono1
//
//  Created by Niblit on 2012-07-02.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "SysTypes.h"
#include "MonoFwd.h"
#include <string>

namespace mono
{
    struct TextDefinition
    {
        FloatVector vertices;
        FloatVector texcoords;
        Color color;
        int chars;
    };

    typedef std::vector<TextDefinition> TextDefCollection;
    
    //! @param font Font file to use
    //! @param size Size of the font on the generated bitmap
    //! @param scale Scale of font when drawing with opengl (small font sizes can produce bad quality)
    void LoadFont(const std::string& font, float size, float scale = 1.0f);

    //! @param text The text...
    //! @param pos The position...
    //! @param center If the text should be horizontally centered to the position
    //! @return TextDefinition
    TextDefinition GenerateVertexDataFromString(const std::string& text, const Math::Vector2f& pos, bool center);
    
    //! Draw all the texts defined at once
    //! @param collection A collection of text definitions
    void DrawTextFromDefinitions(const TextDefCollection& collection);
}

