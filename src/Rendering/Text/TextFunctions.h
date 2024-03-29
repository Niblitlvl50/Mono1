
#pragma once

#include "TextDefinition.h"
#include "TextFlags.h"
#include "Math/MathFwd.h"
#include "Rendering/Texture/ITextureFactory.h"

namespace mono
{
    //! Loads a font
    //! @param font_id The id of the font that you specify
    //! @param font Font file to use
    //! @param size Size of the font on the generated bitmap
    //! @param scale Scale of font when drawing with opengl (small font sizes can produce bad quality)
    void LoadFont(int font_id, const char* font_file, float size, float scale);
    void LoadFontRaw(int font_id, const unsigned char* data_bytes, int data_size, float size, float scale);

    void UnloadFonts();

    //! Get the loaded font texture, might be nullptr if no texture is loaded.
    ITexturePtr GetFontTexture(int font_id);

    math::Vector TextOffsetFromFontCentering(const math::Vector& text_size, FontCentering center_flags);
    TextDefinition GenerateVertexDataFromString(int font_id, const char* text, FontCentering center_flags);

    struct TextMeasurement
    {
        math::Vector size;
        math::Vector offset;
    };
    TextMeasurement MeasureString(int font_id, const char* text);
}
