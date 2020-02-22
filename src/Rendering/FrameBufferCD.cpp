
#include "FrameBufferCD.h"
#include "Texture/ITextureFactory.h"
#include "System/open_gl.h"

using namespace mono;

FrameBufferCD::FrameBufferCD(int width, int height)
{
    m_texture = mono::GetTextureFactory()->CreateTexture(nullptr, width, height, 4);
}
