
#pragma once

#include "MonoPtrFwd.h"

namespace mono
{
    class FrameBufferCD
    {
    public:

        FrameBufferCD(int width, int height);
    
        ITexturePtr m_texture;
    };
}
