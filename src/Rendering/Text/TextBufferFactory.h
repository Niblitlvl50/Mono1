
#pragma once

#include "TextFlags.h"
#include "Rendering/RenderFwd.h"
#include <memory>

namespace mono
{
    struct TextDrawBuffers
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> uv;
        std::unique_ptr<IElementBuffer> indices;
    };

    TextDrawBuffers BuildTextDrawBuffers(int font_id, const char* text, mono::FontCentering center_flags);
}
