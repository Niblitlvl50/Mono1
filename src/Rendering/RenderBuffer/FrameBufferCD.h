
#pragma once

#include "Math/Vector.h"
#include "Rendering/Texture/ITextureFactory.h"

namespace mono
{
    class FrameBuffer
    {
    public:

        FrameBuffer(int width, int height);
        ~FrameBuffer();

        const math::Vector& Size() const;
        mono::ITexturePtr Texture() const;
        void Use();

    private:
        const math::Vector m_size;
        ITexturePtr m_texture;
        unsigned int m_fbo;
        //unsigned int m_fbo_texture;
        unsigned int m_rbo;
    };

    void ClearFrameBuffer();
}
