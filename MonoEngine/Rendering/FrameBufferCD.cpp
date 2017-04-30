
#include "FrameBufferCD.h"
#include "System/OpenGL.h"
#include "Texture/TextureFactory.h"

using namespace mono;

FrameBufferCD::FrameBufferCD(int width, int height)
{
    m_texture = mono::CreateTexture(nullptr, width, height, 4);
}
