
#include "TextFunctions.h"
#include "System/File.h"
#include "Rendering/RenderSystem.h"
#include "Rendering/Texture/ITexture.h"
#include "Rendering/Texture/ITextureFactory.h"
#include "Math/Vector.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"

#include <cstring>
#include <cstdio>
#include <unordered_map>
#include <cassert>

namespace
{
    constexpr int g_extended_ascii_table_size = 256;
    constexpr int g_ascii_printable_char_offset = 32;

    constexpr int g_base = g_ascii_printable_char_offset;
    constexpr int g_n_chars = g_extended_ascii_table_size - g_ascii_printable_char_offset;

    struct CharData
    {
        float x_advance;
        float x_offset;
        float y_offset;
        float width;
        float height;
        
        float uv_x0;
        float uv_y0;
        float uv_x1;
        float uv_y1;
    };

    struct FontData
    {
        mono::ITexturePtr texture;
        CharData chars[g_n_chars];
    };

    std::unordered_map<int, FontData> g_fonts;
}

void mono::LoadFont(int font_id, const char* font_file, float size, float scale)
{
    file::FilePtr font = file::OpenBinaryFile(font_file);
    
    const std::vector<byte> font_buffer = file::FileRead(font);
    LoadFontRaw(font_id, font_buffer.data(), font_buffer.size(), size, scale);
}

void mono::LoadFontRaw(int font_id, const unsigned char* data_bytes, int data_size, float size, float scale)
{
    constexpr int width = 512;
    constexpr int height = 512;
    constexpr float uv_x_multi = 1.0f / width;
    constexpr float uv_y_multi = 1.0f / height;

    byte bitmap_data[width * height];
    stbtt_bakedchar char_data[g_n_chars];
    
    // The bitmap generated by this function has origo in the upper left corner,
    // thats why a bit down the y texture coordinate is flipped.
    stbtt_BakeFontBitmap(data_bytes, 0, size, bitmap_data, width, height, g_base, g_n_chars, char_data);

    FontData font_data;
    font_data.texture = mono::RenderSystem::GetTextureFactory()->CreateTexture(bitmap_data, width, height, 1);

    for(int index = 0; index < g_n_chars; ++index)
    {
        const stbtt_bakedchar& baked_char = char_data[index];
        
        CharData& data = font_data.chars[index];
        data.width = (baked_char.x1 - baked_char.x0) * scale;
        data.height = (baked_char.y1 - baked_char.y0) * scale;
        data.x_advance = baked_char.xadvance * scale;
        data.x_offset = baked_char.xoff * scale;
        data.y_offset = (baked_char.yoff * scale) + data.height;
        data.uv_x0 = baked_char.x0 * uv_x_multi;
        data.uv_y0 = baked_char.y1 * uv_y_multi;
        data.uv_x1 = baked_char.x1 * uv_x_multi;
        data.uv_y1 = baked_char.y0 * uv_y_multi;
    }

    g_fonts.insert(std::make_pair(font_id, font_data));
}

void mono::UnloadFonts()
{
    g_fonts.clear();
}

mono::ITexturePtr mono::GetFontTexture(int font_id)
{
    return g_fonts.find(font_id)->second.texture;
}

math::Vector mono::TextOffsetFromFontCentering(const math::Vector& text_size, mono::FontCentering center_flags)
{
    math::Vector offset = math::ZeroVec;
    math::Vector text_half_size = text_size / 2.0f;

    if(center_flags & FontCentering::HORIZONTAL)
        offset.x -= text_half_size.x;

    if(center_flags & FontCentering::VERTICAL)
        offset.y -= text_half_size.y;

    return offset;
}

mono::TextDefinition mono::GenerateVertexDataFromString(int font_id, const char* text, FontCentering center_flags)
{
    const uint32_t text_length = std::strlen(text);

    mono::TextDefinition text_def;
    text_def.vertices.reserve(text_length * 4);
    text_def.uv_coords.reserve(text_length * 4);
    text_def.indices.reserve(text_length * 6);

    math::Vector current_position = math::ZeroVec;
    if(center_flags != 0)
    {
        const mono::TextMeasurement text_measurement = mono::MeasureString(font_id, text);
        current_position += TextOffsetFromFontCentering(text_measurement.size, center_flags);
    }

    const FontData& font_data = g_fonts.find(font_id)->second;

    for(uint32_t index = 0; index < text_length; ++index)
    {
        const char text_char = text[index];
        uint32_t char_index = static_cast<uint32_t>(text_char);
        if(char_index < g_ascii_printable_char_offset || char_index > g_n_chars)
            char_index = 35;

        // For the correct char we need to add base to the index.
        const uint32_t offset_index = char_index - g_base;
        const CharData& data = font_data.chars[offset_index];

        const float x0 = current_position.x + data.x_offset;
        const float y0 = current_position.y - data.y_offset;
        const float x1 = x0 + data.width;
        const float y1 = y0 + data.height;

        text_def.vertices.emplace_back(x0, y0);
        text_def.vertices.emplace_back(x0, y1);
        text_def.vertices.emplace_back(x1, y1);
        text_def.vertices.emplace_back(x1, y0);

        text_def.uv_coords.emplace_back(data.uv_x0, data.uv_y0);
        text_def.uv_coords.emplace_back(data.uv_x0, data.uv_y1);
        text_def.uv_coords.emplace_back(data.uv_x1, data.uv_y1);
        text_def.uv_coords.emplace_back(data.uv_x1, data.uv_y0);

        const uint16_t indices_base = index * 4;

        text_def.indices.push_back(indices_base + 0);
        text_def.indices.push_back(indices_base + 1);
        text_def.indices.push_back(indices_base + 2);

        text_def.indices.push_back(indices_base + 0);
        text_def.indices.push_back(indices_base + 2);
        text_def.indices.push_back(indices_base + 3);

        current_position.x += data.x_advance;
    }

    return text_def;
}


mono::TextMeasurement mono::MeasureString(int font_id, const char* text)
{
    const FontData& font_data = g_fonts.find(font_id)->second;
    const uint32_t text_length = std::strlen(text);

    mono::TextMeasurement measurement;
    
    for(uint32_t index = 0; index < text_length; ++index)
    {
        const char text_char = text[index];
        uint32_t char_index = static_cast<uint32_t>(text_char);
        if(char_index < g_ascii_printable_char_offset || char_index > g_n_chars)
            char_index = 35;

        // For the correct char we need to add base to the index.
        const uint32_t offset_index = char_index - g_base;
        const CharData& data = font_data.chars[offset_index];

        measurement.size.x += ((index == text_length -1) ? data.width : data.x_advance);
        measurement.size.y = std::max(data.height, measurement.size.y);
        measurement.offset.y = std::max(data.y_offset, measurement.offset.y);
    }

    return measurement;
}
