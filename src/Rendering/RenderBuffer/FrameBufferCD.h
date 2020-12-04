
#pragma once

#include "Math/Vector.h"
#include "Rendering/Texture/ITextureFactory.h"

#include <cstdint>

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

        bool CopyFrameBufferData(byte* data, uint32_t data_length);

    private:
        const math::Vector m_size;
        ITexturePtr m_texture;
        uint32_t m_fbo;
        uint32_t m_rbo_depth_stencil;
        uint32_t m_rbo_color;

        uint32_t m_pbo;
    };

    void ClearFrameBuffer();
}
