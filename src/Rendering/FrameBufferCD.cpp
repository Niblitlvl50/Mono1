
#include "FrameBufferCD.h"
#include "Texture/TextureFactory.h"
#include "System/open_gl.h"

using namespace mono;

FrameBufferCD::FrameBufferCD(int width, int height)
{
    m_texture = mono::CreateTexture(nullptr, width, height, 4);
}
