
#pragma once

#include "Rendering/RenderPtrFwd.h"

namespace mono
{
    class FrameBufferCD
    {
    public:

        FrameBufferCD(int width, int height);
    
        ITexturePtr m_texture;
    };
}
