
#include "GifWriter.h"
#include "gif-h/gif.h"

namespace
{
    GifWriter g_gif_handle;
    uint32_t g_gif_width;
    uint32_t g_gif_height;
}

using byte = unsigned char;

bool mono::GifWriterBegin(uint32_t width, uint32_t height)
{
    g_gif_handle = {};
    g_gif_width = width;
    g_gif_height = height;

    const bool result = GifBegin(&g_gif_handle, "capture.gif", g_gif_width, g_gif_height, 100);
    return result;
}

bool mono::GifWriterAddFrame(const byte* image_data)
{
    const bool result = GifWriteFrame(&g_gif_handle, image_data, g_gif_width, g_gif_height, 100);
    return result;
}

bool mono::GifWriterEnd()
{
    const bool result = GifEnd(&g_gif_handle);
    return result;
}
