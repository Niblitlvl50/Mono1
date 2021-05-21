
#include "Texture.h"
#include "System/System.h"
#include <cstdio>

using namespace mono;

TextureImpl::TextureImpl(
    uint32_t width, uint32_t height, uint32_t color_components, const unsigned char* image_data)
    : m_width(width)
    , m_height(height)
{
    sg_pixel_format data_format = SG_PIXELFORMAT_R8;
    if(color_components == 2)
        data_format = SG_PIXELFORMAT_RG8;
    //else if(color_components == 3)
    //    data_format = SG_PIXELFORMAT_RGB8;
    else if(color_components == 4)
        data_format = SG_PIXELFORMAT_RGBA8;

    sg_image_desc image_desc = {};
    image_desc.width = width;
    image_desc.height = height;
    image_desc.pixel_format = data_format;
    image_desc.content.subimage[0][0].ptr = image_data;
    image_desc.content.subimage[0][0].size = width * height * color_components * sizeof(unsigned char);

    m_handle = sg_make_image(&image_desc);

    const sg_resource_state state = sg_query_image_state(m_handle);
    if(state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create texture.");
}

TextureImpl::TextureImpl(sg_image image_handle)
    : m_handle(image_handle)
{
    const sg_resource_state state = sg_query_image_state(m_handle);
    if(state != SG_RESOURCESTATE_VALID)
        System::Log("Failed to create texture.");

    const sg_image_info info = sg_query_image_info(m_handle);
    m_width = info.width;
    m_height = info.height;
}

TextureImpl::~TextureImpl()
{
    sg_destroy_image(m_handle);
}

uint32_t TextureImpl::Width() const
{
    return m_width;
}

uint32_t TextureImpl::Height() const
{
    return m_height;
}

uint32_t TextureImpl::Id() const
{
    return m_handle.id;
}
