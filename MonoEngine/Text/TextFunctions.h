//
//  TextFunctions.h
//  Mono1
//
//  Created by Niblit on 2012-07-02.
//  Copyright 2012 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "TextDefinition.h"
#include "MathFwd.h"
#include <string>

namespace mono
{    
    //! Loads a font
    //! @param font Font file to use
    //! @param size Size of the font on the generated bitmap
    //! @param scale Scale of font when drawing with opengl (small font sizes can produce bad quality)
    void LoadFont(const std::string& font, float size, float scale = 1.0f);

    //! Use the loaded font
    void UseFont();

    //! Clear the loaded font
    void UnloadFont();

    //! Generates a text definition
    //! @param text The text...
    //! @param pos The position...
    //! @param center If the text should be horizontally centered to the position
    //! @return TextDefinition
    TextDefinition GenerateVertexDataFromString(const std::string& text, const math::Vector2f& pos, bool center);
    
    //! Measures a string
    //! @param text The text to measure
    //! @return Size of the string
    math::Vector2f MeasureString(const std::string& text);
}

