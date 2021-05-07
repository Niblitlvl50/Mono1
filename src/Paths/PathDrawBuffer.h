
#pragma once

#include "Rendering/RenderFwd.h"
#include <memory>

namespace mono
{
    struct PathDrawBuffer
    {
        std::unique_ptr<IRenderBuffer> vertices;
        std::unique_ptr<IRenderBuffer> colors;
        std::unique_ptr<IRenderBuffer> anotations;
        std::unique_ptr<IElementBuffer> indices;
    };
}
