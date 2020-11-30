
#pragma once

#include <cstdint>

using byte = unsigned char;

namespace mono
{
    bool GifWriterBegin(uint32_t width, uint32_t height);
    bool GifWriterAddFrame(const byte* image_data);
    bool GifWriterEnd();
}
